package main

import "testing"

func TestObfuscateMapEmpty(t *testing.T) {
	source := []rune("")
	target := []rune("")

	m := createObfuscateMap(source, target)

	if len(m) != 0 {
		t.Errorf("Failed")
	}
}

func TestUnobfuscateMapEmpty(t *testing.T) {
	source := []rune("")
	target := []rune("")

	m := createUnobfuscateMap(source, target)

	if len(m) != 0 {
		t.Errorf("Failed")
	}
}

func TestObfuscateMap(t *testing.T)  {
	source := []rune("A")
	target := []rune("B")

	m := createObfuscateMap(source, target)

	if m['A'] != 'B' {
		t.Errorf("Failed")
	}
}

func TestUnobfuscateMap(t *testing.T)  {
	source := []rune("A")
	target := []rune("B")

	m := createUnobfuscateMap(source, target)

	if m['B'] != 'A' {
		t.Errorf("Failed")
	}
}

func TestObfuscate(t *testing.T)  {
	source := []rune("A")
	target := []rune("B")

	var str_obs string_obfuscator
	str_obs.init(source, target)

	obf := str_obs.obfuscate("B")

	if obf != "A" {
		t.Errorf(string(len(str_obs.obfuscateMap)))
	}
}

func TestUnobfuscate(t *testing.T)  {
	source := []rune("A")
	target := []rune("B")

	var str_obs string_obfuscator
	str_obs.init(source, target)

	if str_obs.unobfuscate("A") != "B" {
		t.Errorf("Failed")
	}
}