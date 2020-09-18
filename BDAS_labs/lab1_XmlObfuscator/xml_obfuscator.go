package main

import (
	"bytes"
	"encoding/xml"
	"io/ioutil"
	"log"
)

type work_mode int

const (
	obfuscate   work_mode = 1
	unobfuscate work_mode = 2
)

type Node struct {
	XMLName xml.Name
	Attrs   []xml.Attr `xml:",attr"`
	Content []byte     `xml:",innerxml"`
	Nodes   []Node     `xml:",any"`
}

func (n *Node) UnmarshalXML(d *xml.Decoder, start xml.StartElement) error {
	n.Attrs = start.Attr
	type node Node

	return d.DecodeElement((*node)(n), &start)
}

func walk(nodes []Node, f func(*Node) bool) {
	for i := range nodes {
		if f(&nodes[i]) {
			walk(nodes[i].Nodes, f)
		}
	}
}

func obfuscate_xml(input_file, output_file string, str_obs string_obfuscator, wm work_mode) {
	data, err := ioutil.ReadFile(input_file)
	if err != nil {
		log.Fatal(err)
	}

	buf := bytes.NewBuffer(data)
	dec := xml.NewDecoder(buf)

	var n Node
	err = dec.Decode(&n)
	if err != nil {
		panic(err)
	}

	work_fun := func(s string) string { return "" }
	if wm == obfuscate {
		work_fun = str_obs.obfuscate
	} else if wm == unobfuscate {
		work_fun = str_obs.unobfuscate
	}

	arr := []Node{n}

	walk(arr, func(n *Node) bool {
		if len(n.Nodes) == 0 {
			n.Content = []byte(work_fun(string(n.Content)))
		} else {
			n.Content = []byte{}
		}
		return true
	})

	n = arr[0]

	file, err := xml.MarshalIndent(n, "", "\t")
	if err != nil {
		panic(err)
	}

	err = ioutil.WriteFile(output_file, file, 0644)
	if err != nil {
		panic(err)
	}
}
