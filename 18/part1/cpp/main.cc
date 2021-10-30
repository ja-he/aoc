#include <iomanip>
#include <iostream>

#include <algorithm>
#include <memory>
#include <numeric>

#include <bitset>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <cassert>
#include <cctype>

enum TokenType
{
  ILLEGAL,
  EOL,
  LPAREN,
  RPAREN,
  NUMBER,
  OPERATOR_ADD,
  OPERATOR_MULTIPLY
};

std::string
to_string(TokenType t)
{
  switch (t)
  {
    case ILLEGAL: return "ILLEGAL";
    case EOL: return "<eol>";
    case LPAREN: return "(";
    case RPAREN: return ")";
    case NUMBER: return "<num>";
    case OPERATOR_ADD: return "+";
    case OPERATOR_MULTIPLY: return "*";
  }
}

struct Token
{
  TokenType   type;
  std::string content;

  void clear(void)
  {
    this->type    = ILLEGAL;
    this->content = "";
  }
  bool empty(void) const
  {
    return (this->type == ILLEGAL && this->content.empty());
  }
  std::string to_string(void) const
  {
    return "[" + ::to_string(this->type) + " '" + this->content + "']";
  }
};

using TokenizedExpression = std::vector<Token>;

enum Precedence
{
  PREC_LOWEST   = 0,
  PREC_ADD      = 1,
  PREC_MULTIPLY = 1,
};

Precedence
get_precedence(TokenType t)
{
  switch (t)
  {
    case OPERATOR_ADD: return PREC_ADD;
    case OPERATOR_MULTIPLY: return PREC_MULTIPLY;
    default: return PREC_LOWEST;
  }
}

TokenizedExpression
tokenize(const std::string& expression)
{
  TokenizedExpression tokens;
  Token               current_token;
  current_token.clear();
  for (int i = 0; i < expression.length(); i++)
  {
    switch (expression[i])
    {
      case ' ':
      {
        if (!current_token.empty())
        {
          assert(current_token.type != ILLEGAL);
          tokens.push_back(current_token);
          current_token.clear();
        }
        continue;
      }
      case '(':
      {
        if (!current_token.empty())
        {
          tokens.push_back(current_token);
          current_token.clear();
        }
        tokens.push_back({ LPAREN, "(" });
        continue;
      }
      case ')':
      {
        if (!current_token.empty())
        {
          tokens.push_back(current_token);
          current_token.clear();
        }
        tokens.push_back({ RPAREN, ")" });
        continue;
      }
      case '+':
      {
        if (!current_token.empty())
        {
          tokens.push_back(current_token);
          current_token.clear();
        }
        tokens.push_back({ OPERATOR_ADD, "+" });
        continue;
      }
      case '*':
      {
        if (!current_token.empty())
        {
          tokens.push_back(current_token);
          current_token.clear();
        }
        tokens.push_back({ OPERATOR_MULTIPLY, "*" });
        continue;
      }
      default:
      {
        if (current_token.empty())
          if (std::isdigit(expression[i]))
            current_token.type = NUMBER;
        assert(current_token.type == NUMBER);
        current_token.content += expression[i];
      }
    }
  }
  if (!current_token.empty())
  {
    tokens.push_back(current_token);
  }
  tokens.push_back({ EOL, "" });
  return tokens;
}

using Value = uint64_t;
class BinaryOperation;
using Expression = std::variant<BinaryOperation, Value>;

class BinaryOperation
{
public:
  BinaryOperation(const Expression* lhs, TokenType op, const Expression* rhs)
    : lhs(lhs)
    , op(op)
    , rhs(rhs){};

  const Expression* lhs;
  TokenType         op;
  const Expression* rhs;
};

[[nodiscard]] Value
eval(const Expression* e)
{
  bool is_bop   = e->index() == 0;
  bool is_value = e->index() == 1;
  if (is_bop)
  {
    auto bop = std::get<BinaryOperation>(*e);
    switch (bop.op)
    {
      case OPERATOR_ADD: return eval(bop.lhs) + eval(bop.rhs);
      case OPERATOR_MULTIPLY: return eval(bop.lhs) * eval(bop.rhs);
      default:
        std::cerr << "error: invalid binary operation '" << to_string(bop.op)
                  << "'" << std::endl;
        exit(1);
    }
  }
  else if (is_value)
  {
    return std::get<Value>(*e);
  }
  else
  {
    std::cerr << "error: variant index is " << e->index() << std::endl;
    exit(1);
  }
};

[[nodiscard]] std::string
to_string(const Expression* e)
{
  bool is_bop   = e->index() == 0;
  bool is_value = e->index() == 1;
  if (is_bop)
  {
    auto bop = std::get<BinaryOperation>(*e);
    return "(" + to_string(bop.lhs) + to_string(bop.op) + to_string(bop.rhs) +
           ")";
  }
  else if (is_value)
  {
    return std::to_string(std::get<Value>(*e));
  }
  else
  {
    std::cerr << "error: variant index is " << e->index() << std::endl;
    exit(1);
  }
};

using InfixParsingFunction = std::function<Expression(Expression)>;

class PrattParser
{
public:
  PrattParser(const TokenizedExpression& tokens)
    : tokens(tokens)
  {
    final_expression = parseExpression(PREC_LOWEST);
  };

  Expression* final_expression;

private:
  void         advance(void) { this->current_token_index++; };
  const Token& peek(void) const
  {
    assert(current_token_index + 1 < tokens.size());
    return this->tokens[this->current_token_index + 1];
  };
  const Token& current(void) const
  {
    assert(current_token_index < tokens.size());
    return this->tokens[this->current_token_index];
  };

  const TokenizedExpression& tokens;
  size_t                     current_token_index = 0;

private:
  auto parseExpression(int precedence) -> Expression*;
  auto parseInfixExpression(Expression* lhs) -> Expression*;
  auto parseGroupedExpression() -> Expression*;
  auto parseValue() -> Expression*;
};

auto
PrattParser::parseValue() -> Expression*
{
  /*   std::cerr << "parsing value " << current().to_string() << std::endl; */
  assert(this->current().type == NUMBER);

  return new Expression{ (Value)std::stoul(current().content) };
};

auto
PrattParser::parseGroupedExpression() -> Expression*
{
  /*   std::cerr << "parsing grouped " << current().to_string() << std::endl; */
  advance();

  auto e = parseExpression(PREC_LOWEST);
  advance();
  assert(current().type == RPAREN || [this]() { return false; }());
  return e;
};

auto
PrattParser::parseInfixExpression(Expression* lhs) -> Expression*
{
  /*   std::cerr << "parsing infix expr " << current().to_string() << std::endl;
   */
  auto infix_op = current().type;

  auto current_precedence = get_precedence(current().type);
  advance();
  Expression* rhs = parseExpression(current_precedence);

  return new Expression(BinaryOperation{ lhs, infix_op, rhs });
};

auto
PrattParser::parseExpression(int precedence) -> Expression*
{
  /*   std::cerr << "parsing expr " << current().to_string() << std::endl; */
  Expression* lhs;
  switch (current().type)
  {
    case LPAREN: lhs = parseGroupedExpression(); break;
    case NUMBER: lhs = parseValue(); break;
    default: // TODO: ERROR
      ;
  }

  auto at_end                   = [this]() { return peek().type == EOL; };
  auto greater_precedence_ahead = [this, &precedence]()
  { return precedence < get_precedence(peek().type); };

  while (!at_end() && greater_precedence_ahead())
  {
    advance();
    switch (current().type)
    {
      case OPERATOR_ADD: lhs = parseInfixExpression(lhs); break;
      case OPERATOR_MULTIPLY: lhs = parseInfixExpression(lhs); break;
      default:;
    }
  }

  return lhs;
};

int
main(void)
{

  // get linewise input
  std::vector<std::string> input;
  {
    std::string line;
    while (std::getline(std::cin, line)) { input.push_back(line); }
  }

  // tokenize input
  std::vector<TokenizedExpression> tokenized_expressions;
  std::transform(input.begin(),
                 input.end(),
                 std::back_inserter(tokenized_expressions),
                 [](const std::string& expression)
                 { return tokenize(expression); });

  // debug print tokens
  uint64_t sum = 0;
  for (int i = 0; i < input.size(); i++)
  {
    PrattParser p{ tokenized_expressions[i] };
    sum += eval(p.final_expression);
  }

  std::cout << sum << std::endl;
}
