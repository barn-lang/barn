class Token {
  constructor(type, value) {
    this.type = type;
    this.value = value;
  }
}

class Lexer {
  constructor(input) {
    this.input = input;
    this.currentChar = input[0];
    this.position = 0;
  }

  advance() {
    this.position++;
    if (this.position < this.input.length) {
      this.currentChar = this.input[this.position];
    } else {
      this.currentChar = null;
    }
  }

  skipWhitespace() {
    while (this.currentChar !== null && /\s/.test(this.currentChar)) {
      this.advance();
    }
  }

  getNextToken() {
    while (this.currentChar !== null) {
      if (/\s/.test(this.currentChar)) {
        this.skipWhitespace();
        continue;
      }

      if (/\d/.test(this.currentChar)) {
        let num = '';
        while (this.currentChar !== null && /\d/.test(this.currentChar)) {
          num += this.currentChar;
          this.advance();
        }
        return new Token('NUMBER', parseInt(num));
      }

      if (this.currentChar === '+') {
        this.advance();
        return new Token('PLUS', '+');
      }

      if (this.currentChar === '-') {
        this.advance();
        return new Token('MINUS', '-');
      }

      if (this.currentChar === '(') {
	this.advance();
   return new Token('LPAREN', '('); 
}

if (this.currentChar === ')') {
	this.advance();
   return new Token('RPAREN', ')'); 
}

      throw new Error(`Invalid character: ${this.currentChar}`);
    }

    return new Token('EOF', null);
  }
}

class Parser {
  constructor(lexer) {
    this.lexer = lexer;
    this.currentToken = this.lexer.getNextToken();
  }

  eat(tokenType) {
    if (this.currentToken.type === tokenType) {
      this.currentToken = this.lexer.getNextToken();
    } else {
      throw new Error(`Expected ${tokenType}, found ${this.currentToken.type}`);
    }
  }

  factor() {
    const token = this.currentToken;
    if (token.type === 'NUMBER') {
      this.eat('NUMBER');
      return { type: 'Number', value: token.value };
    } else if (token.type === 'LPAREN') {
      this.eat('LPAREN');
      const node = this.expr();
      this.eat('RPAREN');
      return node;
    } else {
      throw new Error(`Unexpected token: ${token.type}`);
    }
  }

  term() {
    let node = this.factor();
    while (this.currentToken.type === 'MUL' || this.currentToken.type === 'DIV') {
      const token = this.currentToken;
      if (token.type === 'MUL') {
        this.eat('MUL');
      } else {
        this.eat('DIV');
      }
      node = { type: 'BinaryOperation', left: node, operator: token.value, right: this.factor() };
    }
    return node;
  }

  expr() {
    let node = this.term();
    while (this.currentToken.type === 'PLUS' || this.currentToken.type === 'MINUS') {
      const token = this.currentToken;
      if (token.type === 'PLUS') {
        this.eat('PLUS');
      } else {
        this.eat('MINUS');
      }
      node = { type: 'BinaryOperation', left: node, operator: token.value, right: this.term() };
    }
    return node;
  }

  parse() {
    return this.expr();
  }
}

// Example usage:
const input = '2 - 3 - (4 - 1)';
const lexer = new Lexer(input);
const parser = new Parser(lexer);
const ast = parser.parse();
console.log(JSON.stringify(ast, null, 2));

