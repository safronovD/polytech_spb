package com.example.bc_app;

import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.*;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.bouncycastle.asn1.ASN1InputStream;
import org.bouncycastle.asn1.cms.ContentInfo;
import org.bouncycastle.cert.X509CertificateHolder;
import org.bouncycastle.cert.jcajce.JcaCertStore;
import org.bouncycastle.cms.*;
import org.bouncycastle.cms.jcajce.*;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.operator.ContentSigner;
import org.bouncycastle.operator.OutputEncryptor;
import org.bouncycastle.operator.jcajce.JcaContentSignerBuilder;
import org.bouncycastle.operator.jcajce.JcaDigestCalculatorProviderBuilder;
import org.bouncycastle.util.Store;

public class MainClass {

    public static void main(String[] args) {
        Security.setProperty("crypto.policy", "unlimited");

        try {
            int maxKeySize = javax.crypto.Cipher.getMaxAllowedKeyLength("AES");
            System.out.println("Max Key Size for AES : " + maxKeySize);

            Security.addProvider(new BouncyCastleProvider());

            Path certPath = Paths.get("data", "public.cer");
            Path keyPath = Paths.get("data", "private.p12");
            Path dataPath = Paths.get("data", "input_data.txt");

            CertificateFactory certFactory = CertificateFactory
                    .getInstance("X.509", "BC");
            X509Certificate certificate = (X509Certificate) certFactory
                    .generateCertificate(new FileInputStream(certPath.toString()));
            char[] keystorePassword = "password".toCharArray();
            char[] keyPassword = "password".toCharArray();

            KeyStore keystore = KeyStore.getInstance("PKCS12");
            keystore.load(new FileInputStream(keyPath.toString()), keystorePassword);
            PrivateKey privateKey = (PrivateKey) keystore.getKey("baeldung",
                    keyPassword);

            byte[] dataToEncrypt = Files.readAllBytes(dataPath);
            System.out.println("Original Message : " + new String(dataToEncrypt));
            byte[] encryptedData = encryptData(dataToEncrypt, certificate);
            System.out.println("Encrypted Message :");
            System.out.println(new String(encryptedData));
            byte[] rawData = decryptData(encryptedData, privateKey);
            System.out.println("Decrypted Message : " + new String(rawData));

            byte[] signedData = signData(rawData, certificate, privateKey);
            System.out.println("Signed Data :");
            System.out.println(new String(signedData));
            Boolean check = verifySignedData(signedData);
            System.out.println("Signed Data validation : " + check);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static byte[] encryptData(byte[] data,
                                     X509Certificate encryptionCertificate)
            throws CMSException, IOException, CertificateEncodingException {
        byte[] encryptedData = null;
        if (null != data && null != encryptionCertificate) {
            CMSEnvelopedDataGenerator cmsEnvelopedDataGenerator
                    = new CMSEnvelopedDataGenerator();
            JceKeyTransRecipientInfoGenerator jceKey
                    = new JceKeyTransRecipientInfoGenerator(encryptionCertificate);
            cmsEnvelopedDataGenerator.addRecipientInfoGenerator(jceKey);
            CMSTypedData msg = new CMSProcessableByteArray(data);
            OutputEncryptor encryptor
                    = new JceCMSContentEncryptorBuilder(CMSAlgorithm.AES128_CBC)
                    .setProvider("BC").build();
            CMSEnvelopedData cmsEnvelopedData = cmsEnvelopedDataGenerator
                    .generate(msg,encryptor);
            encryptedData = cmsEnvelopedData.getEncoded();
        }
        return encryptedData;
    }

    public static byte[] decryptData(
            byte[] encryptedData,
            PrivateKey decryptionKey)
            throws CMSException {
        byte[] decryptedData = null;
        if (null != encryptedData && null != decryptionKey) {
            CMSEnvelopedData envelopedData = new CMSEnvelopedData(encryptedData);
            Collection<RecipientInformation> recipients
                    = envelopedData.getRecipientInfos().getRecipients();
            KeyTransRecipientInformation recipientInfo
                    = (KeyTransRecipientInformation) recipients.iterator().next();
            JceKeyTransRecipient recipient
                    = new JceKeyTransEnvelopedRecipient(decryptionKey);
            return recipientInfo.getContent(recipient);
        }
        return decryptedData;
    }

    public static byte[] signData(
            byte[] data,
            X509Certificate signingCertificate,
            PrivateKey signingKey) throws Exception {
        byte[] signedMessage = null;
        List<X509Certificate> certList = new ArrayList<X509Certificate>();
        CMSTypedData cmsData= new CMSProcessableByteArray(data);
        certList.add(signingCertificate);
        Store certs = new JcaCertStore(certList);
        CMSSignedDataGenerator cmsGenerator = new CMSSignedDataGenerator();
        ContentSigner contentSigner = new JcaContentSignerBuilder("SHA256withRSA")
                .build(signingKey);
        cmsGenerator.addSignerInfoGenerator(new JcaSignerInfoGeneratorBuilder(
                new JcaDigestCalculatorProviderBuilder().setProvider("BC")
                        .build()).build(contentSigner, signingCertificate));
        cmsGenerator.addCertificates(certs);
        CMSSignedData cms = cmsGenerator.generate(cmsData, true);
        signedMessage = cms.getEncoded();
        return signedMessage;
    }

    public static boolean verifySignedData(byte[] signedData)
            throws Exception {
        ByteArrayInputStream inputStream
                = new ByteArrayInputStream(signedData);
        ASN1InputStream asnInputStream = new ASN1InputStream(inputStream);
        CMSSignedData cmsSignedData = new CMSSignedData(
                ContentInfo.getInstance(asnInputStream.readObject()));
        SignerInformationStore signers
                = cmsSignedData.getSignerInfos();
        SignerInformation signer = signers.getSigners().iterator().next();
        Collection<X509CertificateHolder> certCollection
                = cmsSignedData.getCertificates().getMatches(signer.getSID());
        X509CertificateHolder certHolder = certCollection.iterator().next();
        return signer.verify(new JcaSimpleSignerInfoVerifierBuilder()
                .build(certHolder));
    }
}
