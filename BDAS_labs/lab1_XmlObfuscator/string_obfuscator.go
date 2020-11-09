package main

import (
	"unicode"
)

type string_obfuscator struct {
	obfuscateMap   map[rune]rune
	unobfuscateMap map[rune]rune
}

func createObfuscateMap(source, target []rune) map[rune]rune {
	result := make(map[rune]rune)
	for i, val := range source {
		result[val] = target[i]
	}

	return result
}

func createUnobfuscateMap(source, target []rune) map[rune]rune {
	result := make(map[rune]rune)
	for i, val := range target {
		result[val] = source[i]
	}

	return result
}

func (obs *string_obfuscator) init(source, target []rune) {
	obs.obfuscateMap = createObfuscateMap(target, source)
	obs.unobfuscateMap = createUnobfuscateMap(target, source)
}

func (obs string_obfuscator) obfuscate(s string) string {
	result := make([]rune, 0)
	for _, val := range s {
		if unicode.IsDigit(val) || unicode.IsLetter(val) {
			result = append(result, obs.obfuscateMap[val])
		} else {
			result = append(result, val)
		}
	}

	return string(result)
}

func (obs string_obfuscator) unobfuscate(s string) string {
	result := make([]rune, 0)
	for _, val := range s {
		if unicode.IsDigit(val) || unicode.IsLetter(val) {
			result = append(result, obs.unobfuscateMap[val])
		} else {
			result = append(result, val)
		}
	}

	return string(result)
}
