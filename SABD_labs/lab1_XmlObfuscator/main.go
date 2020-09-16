package main

import (
	"fmt"
	"gopkg.in/yaml.v2"
	"os"
	"path"
)

const config_file = "config.yaml"

type Config struct {
	Source                string `yaml:"source"`
	Target                string `yaml:"target"`
	ObfuscateInputFile    string `yaml:"obfuscate_input_file"`
	ObfuscateOutputFile   string `yaml:"obfuscate_output_file"`
	UnobfuscateInputFile  string `yaml:"unobfuscate_input_file"`
	UnobfuscateOutputFile string `yaml:"unobfuscate_output_file"`
	WorkDir               string `yaml:"work_dir"`
}

func NewConfig(configPath string) (*Config, error) {
	s, err := os.Stat(configPath)
	if err != nil {
		return nil, err
	}
	if s.IsDir() {
		return nil, fmt.Errorf("'%s' is a directory, not a normal file", configPath)
	}

	config := &Config{}

	file, err := os.Open(configPath)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	d := yaml.NewDecoder(file)

	if err := d.Decode(&config); err != nil {
		return nil, err
	}

	return config, nil
}

func main() {
	config, err := NewConfig(config_file)
	if err != nil {
		panic(err)
	}

	var str_obs string_obfuscator
	str_obs.init([]rune(config.Source), []rune(config.Target))

	info, err := os.Stat(path.Join(config.WorkDir, config.ObfuscateInputFile))
	if !os.IsNotExist(err) && !info.IsDir() {
		obfuscate_xml(path.Join(config.WorkDir, config.ObfuscateInputFile), path.Join(config.WorkDir, config.ObfuscateOutputFile), str_obs, obfuscate)
	}

	info, err = os.Stat(path.Join(config.WorkDir, config.UnobfuscateInputFile))
	if !os.IsNotExist(err) && !info.IsDir() {
		obfuscate_xml(path.Join(config.WorkDir, config.UnobfuscateInputFile), path.Join(config.WorkDir, config.UnobfuscateOutputFile), str_obs, unobfuscate)
	}
}
