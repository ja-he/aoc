package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
)

type Precedence int

func getPrecedence(t TokenType) Precedence {
	switch t {
	case ADD:
		return 1
	case MULTIPLY:
		return 1
	default:
		return 0
	}
}

type TokenType int

const (
	_ TokenType = iota
	EOL
	NUMBER
	LPAREN
	RPAREN
	ADD
	MULTIPLY
)

func toString(t TokenType) string {
	switch t {
	case EOL:
		return "EOL"
	case NUMBER:
		return "NUMBER"
	case LPAREN:
		return "LPAREN"
	case RPAREN:
		return "RPAREN"
	case ADD:
		return "ADD"
	case MULTIPLY:
		return "MULTIPLY"
	default:
		return "UNKNOWN"
	}
}

type Token struct {
	content string
	tType   TokenType
}

func toToken(rune rune) Token {
	t := Token{content: string(rune)}
	switch rune {
	case '(':
		t.tType = LPAREN
	case ')':
		t.tType = RPAREN
	case '+':
		t.tType = ADD
	case '*':
		t.tType = MULTIPLY
	default:
		t.tType = NUMBER
	}
	return t
}

func tokenize(exprString string) []Token {
	var tokens []Token

	for _, rune := range exprString {
		if rune == ' ' {
			continue
		}
		tokens = append(tokens, toToken(rune))
	}
	tokens = append(tokens, Token{"", EOL})

	return tokens
}

type Expression interface {
	eval() Value
	toString() string
}

type Value uint64

type ValueExpression struct {
	value Value
}

func (v ValueExpression) eval() Value {
	return v.value
}
func (v ValueExpression) toString() string {
	return fmt.Sprint(v.value)
}

type BinaryOperation struct {
	lhs *Expression
	op  TokenType
	rhs *Expression
}

func (bop BinaryOperation) eval() Value {
	switch bop.op {
	case ADD:
		return (*bop.lhs).eval() + (*bop.rhs).eval()
	case MULTIPLY:
		return (*bop.lhs).eval() * (*bop.rhs).eval()
	}
	log.Fatalf("Error: cannot eval '%s' as binary operation", toString(bop.op))
	return 0 // why does it make me put this here when we os.Exit() right before?
}
func (bop BinaryOperation) toString() string {
	str := "(" + (*bop.lhs).toString()
	switch bop.op {
	case ADD:
		str += "+"
	case MULTIPLY:
		str += "*"
	default:
		log.Fatalf("Error: cannot format op '%s' as a string", toString(bop.op))
	}
	str += (*bop.rhs).toString() + ")"
	return str
}

type PrattParser struct {
	tokens []Token
}

func (p *PrattParser) advance()       { p.tokens = p.tokens[1:] }
func (p *PrattParser) peek() Token    { return p.tokens[1] }
func (p *PrattParser) current() Token { return p.tokens[0] }

func (p *PrattParser) parseExpr(precedence Precedence) Expression {
	var lhs Expression
	switch p.current().tType {
	case LPAREN:
		lhs = p.parseGrouped()
	case NUMBER:
		lhs = p.parseValue()
	default:
		log.Fatalf("Error: parseExpr can't handle a '%s'", toString(p.current().tType))
	}

	atEnd := func() bool { return p.peek().tType == EOL }
	greaterPrecAhead := func() bool { return precedence < getPrecedence(p.peek().tType) }

	for !atEnd() && greaterPrecAhead() {
		p.advance()
		switch p.current().tType {
		case ADD:
			lhs = p.parseInfix(lhs)
		case MULTIPLY:
			lhs = p.parseInfix(lhs)
		}
	}

	return lhs
}
func (p *PrattParser) parseGrouped() Expression {
	p.advance()
	e := p.parseExpr(0)
	p.advance()
	if p.current().tType != RPAREN {
		log.Fatalf("Error: after group did not expect token '%s'", toString(p.current().tType))
	}
	return e
}
func (p *PrattParser) parseValue() Expression {
	value, err := strconv.ParseInt(p.current().content, 10, 64)
	if err != nil {
		log.Fatalf("Error: parsing '%s' to int", p.current().content)
	}
	return &ValueExpression{Value(value)}
}
func (p *PrattParser) parseInfix(lhs Expression) Expression {
	op := p.current().tType
	prec := getPrecedence(op)
	p.advance()
	rhs := p.parseExpr(prec)
	bop := &BinaryOperation{lhs: &lhs, op: op, rhs: &rhs}
	return bop
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	sum := Value(0)
	for scanner.Scan() {
		exprString := scanner.Text()
		exprTokens := tokenize(exprString)

		pp := PrattParser{tokens: exprTokens}

		sum += pp.parseExpr(0).eval()
	}

	fmt.Println(sum)
}
