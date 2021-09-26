#include "../Calculator/Calculator.h"

#include <iostream>
#include <string>

using namespace std;

void InputTests();
void CalculatorTests();
void LexerTests();
string ToTableString(string** table, int rowsCount, int columnsCount);

string ForegroundColor(unsigned char r, unsigned char g, unsigned char b)
{
	return "\x1b[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

int main()
{
	setlocale(LC_ALL, "RU");

	//LexerTests();
	//CalculatorTests();
	InputTests();
}

void InputTests()
{
	Calculator calculator;
	std::string input;
	double result = 0;

	while (true)
	{
		cout << ForegroundColor(0xa9, 0xa9, 0xa9) << ">>> ";
		getline(cin, input);
		cout << "-> ";
		try
		{
			result = calculator.Calculate(input);
			cout << ForegroundColor(0x9b, 0x87, 0x0c) << to_string(result);
		}
		catch (std::exception ex)
		{
			cout << ForegroundColor(0x8b, 0x00, 0x00) << ex.what();
		}
		cout << endl;
	}
}

void CalculatorTests()
{
	Calculator calculator;

	const string expressions[] =
	{
		"9 + (9 / 9) + (9 * 9) - 9",              "82",
		"9.9 * 9.5",                              "94,05",
		"(5 + (25 - 4 * 6)) / 2",                 "3",
		"2 + 2 * 2",                              "6",
		"(2 + 2) * 2",                            "8",
		"min(sqrt(sqrt 25 + (81 - 5)), 84) + pi", "12,141593"
	};

	const int rowsCount = sizeof(expressions) / sizeof(string) / 2 + 2;
	const int columnsCount = 3;

	string** table = new string * [rowsCount]();

	table[0] = new string[columnsCount]();
	table[0][0] = "Expression";
	table[0][1] = "Calculated value";
	table[0][2] = "Required value";

	table[1] = nullptr;

	// ----------------------------------

	for (int i = 0; i < rowsCount - 2; i++)
	{
		int row = i + 2;
		table[row] = new string[columnsCount]();
		table[row][0] = expressions[i * 2];
		table[row][1] = to_string(calculator.Calculate(expressions[i * 2]));
		table[row][2] = expressions[i * 2 + 1];
	}

	cout << ToTableString(table, rowsCount, columnsCount);
}

void LexerTests()
{
	Lexer lexer = Lexer(new MemoryStream("(2+2)"));
	while (lexer.MoveNext()) {
		MathToken current = lexer.GetCurrent();
		cout << "Type: " << MathTokenTypes::GetName(current.Type) << " ";
		if (current.Type == MathTokenType::String)
			cout << "Value: " << current.GetStringValue();
		else if (current.Type == MathTokenType::Number)
			cout << "Value: " << current.GetNumberValue();
		cout << endl;
	}
}

string ToTableString(string** table, int rowsCount, int columnsCount)
{
	size_t* columnLengths = new size_t[columnsCount];

	for (int i = 0; i < columnsCount; i++)
		columnLengths[i] = 0;

	for (int i = 0; i < rowsCount; i++)
	{
		string* row = table[i];
		if (!row)
			continue;
		for (int j = 0; j < columnsCount; j++)
		{
			string column = row[j];
			if (column.size() > columnLengths[j])
				columnLengths[j] = column.size();
		}
	}

	string result = "";

	for (int i = 0; i < rowsCount; i++)
	{
		string* row = table[i];
		if (!row)
		{
			result += '|';
			for (int j = 0; j < columnsCount; j++)
			{
				int length = columnLengths[j];
				for (int k = 0; k < length + 2; k++)
					result += '-';
				result += '|';
			}
		}
		else
		{
			result += "| ";
			for (int j = 0; j < columnsCount; j++)
			{
				int length = columnLengths[j];
				string column = row[j];
				result += column;
				for (int k = column.size(); k < length; k++)
					result += ' ';
				if (j != columnsCount - 1)
					result += " | ";
			}
			result += " |";
		}
		if (i != rowsCount - 1)
			result += "\r\n";
	}

	return result;
}