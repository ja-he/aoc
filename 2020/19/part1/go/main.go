package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

type Rule interface {
	matches(message string, offset int) (match bool, numSubrules int)
	getPossibilities() []string
}

type SimpleRule struct {
	matchCharacter byte
}

func (r SimpleRule) matches(message string, offset int) (match bool, numSubrules int) {
	return message[offset] == r.matchCharacter, 1
}
func (r SimpleRule) getPossibilities() []string {
	return []string{fmt.Sprintf("%c", r.matchCharacter)}
}

type Ruleset struct {
	ruleset []Rule
}

func (r Ruleset) matches(message string, offset int) (match bool, numSubrules int) {
	n := 0
	for _, rule := range r.ruleset {
		if match, nn := rule.matches(message, offset); match {
			offset++
			n += nn
		} else {
			return false, 0
		}
	}
	return true, n
}
func (r Ruleset) getPossibilities() []string {
	results := []string{}
	for _, rule := range r.ruleset {
		for i, newSubstr := range rule.getPossibilities() {
			for j := range results {
				results[j] += newSubstr
			}
		}
	}
	return results
}

type CompositeRule struct {
	possibleRules []Ruleset
}

func (r CompositeRule) matches(message string, offset int) (match bool, numSubrules int) {
	for _, possibleRule := range r.possibleRules {
		if match, n := possibleRule.matches(message, offset); match {
			return true, n
		}
	}
	return false, 0
}
func (r CompositeRule) getPossibilities() []string {
	for _, ruleset := range r.possibleRules {
		ruleset.getPossibilities()
		fmt.Println()
	}
}

func NewRuleFromString(line string, sortedRuleStrings *[]string) Rule {
	println("called to construct rule '", line, "'")
	simpleRuleRegex := regexp.MustCompile(`^\d+: "([a-z])"$`)
	compositeRuleFilter := regexp.MustCompile(`^.*: (.*)$`)

	if simpleRuleRegex.MatchString(line) {
		submatches := simpleRuleRegex.FindStringSubmatch(line)
		return SimpleRule{matchCharacter: submatches[1][0]}
	} else {
		result := CompositeRule{}

		rulePartString := compositeRuleFilter.FindStringSubmatch(line)

		rulesetStrings := strings.Split(strings.TrimSpace(rulePartString[1]), "|")
		for _, rulesetString := range rulesetStrings {
			ruleIdxStrs := strings.Split(strings.TrimSpace(rulesetString), " ")

			ruleset := Ruleset{}
			for _, ruleIdxStr := range ruleIdxStrs {
				ruleIndex, err := strconv.ParseInt(ruleIdxStr, 10, 32)
				if err != nil {
					fmt.Println("cant convert '", ruleIdxStr, "' to int")
				}
				ruleset.ruleset = append(ruleset.ruleset, NewRuleFromString((*sortedRuleStrings)[ruleIndex], sortedRuleStrings))
			}

			result.possibleRules = append(result.possibleRules, ruleset)
		}

		return result
	}
}

func NewRules(lines []string) Rule {
	sort.Strings(lines)
	println("constructing rule 0:", lines[0])
	return NewRuleFromString(lines[0], &lines)
}

func main() {
	var ruleLines []string
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		if len(scanner.Text()) == 0 {
			scanner.Scan()
			break
		}
		ruleLines = append(ruleLines, scanner.Text())
	}
	var messages []string
	for scanner.Scan() {
		messages = append(messages, scanner.Text())
	}

	r := NewRules(ruleLines)
	fmt.Println(r)
	r.getPossibilities()

	for _, message := range messages {
		matches, n := r.matches(message, 0)
		if matches && n == len(message) {
			fmt.Println("MATCH:", message)
		} else {
			fmt.Print("-----:", message, " ")
			if !matches {
				fmt.Println("because no match")
			} else {
				fmt.Println("because", (len(message) - n), "extra chars")
			}
		}
	}
}
