#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

class valueError : public std::exception {
public:
	valueError() : std::exception() {};
};

class SyntaxAnalyzer {
public:
	SyntaxAnalyzer();
	void readInitialExpression();
	void analyze();
private:
	std::string initialExpression;
	char ch;
	std::string initialExpressionProcessedPart;
	std::vector<std::string> callStack;
	void read();
	void language();
	void expression();
	void roundBrackets();
	void squareBrackets();
	void operation();
	void afterRoundBrackets();
	void afterSquareBrackets();
	void afterLetters();
	void setOfOperands1();
	void operand();
	void letter();
	void sign();

	bool isSign(const char ch);
	void tryAgain();
	void showCallStack();
};

SyntaxAnalyzer::SyntaxAnalyzer() {
	initialExpression = "";
	initialExpressionProcessedPart = "";
	callStack.push_back("");
}
void SyntaxAnalyzer::readInitialExpression() {
	std::cout << "Правильная скобочная запись арифметических выражений с двумя видами скобок.\n"
		"После круглой скобки в строке может стоять только квадратная, после квадратной - не обязательно.\n"
		"Каждая бинарная операция вместе с операндами берется в скобки.\n"
		"В правильной записи могут присутствовать “лишние” (двойные)\n"
		"скобки, но одна буква не может браться в скобки.\n"
		"\n"
		"Пример.\n"
		"Правильная запись: [[((c+(a+b))*[(c-d)*a])/d]/(([a-b])/[(c+(d-a))*a])]\n"
		"Неправильная запись [(a+b)*([a-b]-([c+a*b]/(a))+b)]\n"
		"\n"
		"Please, enter initial string: ";
	std::cin >> initialExpression;
	read();
}

void SyntaxAnalyzer::read()
{
	callStack.push_back("read(); ");
	if (!initialExpression.empty()) {
		ch = initialExpression[0];
		initialExpressionProcessedPart += ch;
		initialExpression.erase(0, 1);
	}
	else {
		ch = '!';
	}
}

void SyntaxAnalyzer::analyze() {
	try {
		language();
		std::cout << "OK";
	}
	catch (valueError error) {
		showCallStack();
	}
	tryAgain();
}

void SyntaxAnalyzer::language()
{
	callStack.push_back("language(); ");
	if ((ch == '(') || (ch == '[')) {
		expression();
	}
	else throw valueError();
}

void SyntaxAnalyzer::expression()
{
	callStack.push_back("expression(); ");
	if (ch == '(') {
		roundBrackets();
	}
	else if (ch == '[') {
		squareBrackets();
	}
	else throw valueError();
}

void SyntaxAnalyzer::roundBrackets()
{
	callStack.push_back("roundBrackets(); ");
	if (ch == '(') {
		read();
		operation();
		if (ch == ')') {
			read();
		}
		else throw valueError();
	}
	else throw valueError();
}

void SyntaxAnalyzer::squareBrackets()
{
	callStack.push_back("squareBrackets(); ");
	if (ch == '[') {
		read();
		operation();
		if (ch == ']') {
			read();
		}
		else throw valueError();
	}
	else throw valueError();
}

void SyntaxAnalyzer::operation()
{
	callStack.push_back("operation(); ");
	if (ch == '(') {
		roundBrackets();
		afterRoundBrackets();
	}
	else if (std::isalpha(ch)) {
		letter();
		afterLetters();
	}
	else if (ch == '[') {
		squareBrackets();
		afterSquareBrackets();
	}
	else throw valueError();
}

void SyntaxAnalyzer::afterRoundBrackets()
{
	callStack.push_back("afterRoundBrackets(); ");
	if (isSign(ch)) {
		sign();
		if (std::isalpha(ch) || ch == '[') {
			setOfOperands1();
		}
		else throw valueError();
	}
}

void SyntaxAnalyzer::afterSquareBrackets()
{
	callStack.push_back("afterSquareBrackets(); ");
	if (isSign(ch)) {
		sign();
		operand();
	}
}

void SyntaxAnalyzer::afterLetters()
{
	callStack.push_back("afterLetters(); ");
	if (isSign(ch)) {
		sign();
		operand();
	}
	else throw valueError();
}

void SyntaxAnalyzer::setOfOperands1()
{
	callStack.push_back("setOfOperands1(); ");
	if (ch == '[') {
		squareBrackets();
	}
	else if (std::isalpha(ch)) {
		letter();
	}
	else throw valueError();
}

void SyntaxAnalyzer::operand()
{
	callStack.push_back("operand(); ");
	if (ch == '(') {
		roundBrackets();
	}
	else if (ch == '[') {
		squareBrackets();
	}
	else if (std::isalpha(ch)) {
		letter();
	}
	else throw valueError();
}

void SyntaxAnalyzer::letter()
{
	callStack.push_back("letter(); ");
	if (std::isalpha(ch)) {
		read();
	}
	else throw valueError();
}

void SyntaxAnalyzer::sign()
{
	callStack.push_back("sign(); ");
	if (isSign(ch)) {
		read();
	}
	else throw valueError();
}

bool SyntaxAnalyzer::isSign(const char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
		return true;
	else
		return false;
}

void SyntaxAnalyzer::tryAgain()
{
	initialExpressionProcessedPart.clear();
	callStack.clear();
	std::string tryAgain;
	std::cout << "\nTry again?: (Y/N) ";
	std::cin >> tryAgain;
	if (tryAgain == "Y") {
		readInitialExpression();
		analyze();
	}
}

void SyntaxAnalyzer::showCallStack()
{
	std::cout << "Error!\nProcessed part : " << initialExpressionProcessedPart << std::endl << std::endl << "Call stack: ";
	for (std::string call : callStack) {
		std::cout << call;
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzer.readInitialExpression();
	syntaxAnalyzer.analyze();
}