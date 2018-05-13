#include "LLTableBuilderLibrary/LLTableBuilder.h"
#include "LLTableBuilderLibrary/Table/Table.h"
#include "LLTableBuilderLibrary/Table/TableRow/TableRow.h"
#include <iostream>
#include <algorithm>
#include <vector>

const int REQUIRED_ARGC = 1;
const std::string NO_ARGUMENT_ERROR = "Error: rule file is not specified";

std::string SetToString(std::unordered_set<std::string> const & set)
{
	if (set.empty())
	{
		return "";
	}
	std::string result;
	for (std::string const & setElement : set)
	{
		result += setElement;
		result.push_back(',');
	}
	result.erase(result.end() - 1);

	return result;
}

void GetTableRows(Table const & table, std::vector<std::pair<unsigned int, TableRow *>> & rows)
{
	unsigned int currentRowId = 1;
	TableRow * currentRow = table.GetRow(currentRowId);
	while (currentRow != nullptr)
	{
		rows.emplace_back(std::make_pair(currentRowId, currentRow));
		++currentRowId;
		currentRow = table.GetRow(currentRowId);
	}
}

void PrintTable(Table const & table, std::ostream & ostream)
{
	std::vector<std::pair<unsigned int, TableRow *>> rows;
	GetTableRows(table, rows);
	std::sort(rows.begin(), rows.end());

	size_t referencingSetColumnWidth = 14;
	for (auto const & tableElement : rows)
	{
		TableRow * tableRow = tableElement.second;

		referencingSetColumnWidth =
				std::max(referencingSetColumnWidth, SetToString(tableRow->referencingSet).length());
	}

	ostream << "|Id|";
	ostream.width(referencingSetColumnWidth);
	ostream << "ReferencingSet";
	ostream << "|NextId|DoShift|PushToStack|IsError|IsEnd|\n";
	ostream << std::string(referencingSetColumnWidth, '-');
	ostream << "----------------------------------------------\n";

	for (auto const & tableElement : rows)
	{
		unsigned int tableRowId = tableElement.first;
		TableRow * tableRow = tableElement.second;

		ostream << "|";
		ostream.width(2);
		ostream << tableRowId;
		ostream.width(1);
		ostream << "|";
		ostream.width(referencingSetColumnWidth);
		ostream << SetToString(tableRow->referencingSet);
		ostream.width(1);
		ostream << "|";
		ostream.width(6);
		ostream << (tableRow->nextId == 0 ? "-" : std::to_string(tableRow->nextId));
		ostream.width(1);
		ostream << "|";
		ostream.width(7);
		ostream << (tableRow->doShift ? "+" : "-");
		ostream.width(1);
		ostream << "|";
		ostream.width(11);
		ostream << (tableRow->pushToStack == 0 ? "-" : std::to_string(tableRow->pushToStack));
		ostream.width(1);
		ostream << "|";
		ostream.width(7);
		ostream << (tableRow->isError ? "+" : "-");
		ostream.width(1);
		ostream << "|";
		ostream.width(5);
		ostream << (tableRow->isEnd ? "+" : "-");
		ostream.width(1);
		ostream << "|\n";
	}
}

int main(int argc, char * argv[])
{
	if (argc - 1 < REQUIRED_ARGC)
	{
		std::cerr << NO_ARGUMENT_ERROR << "\n";
		return EXIT_FAILURE;
	}
	std::string inputFileName = argv[1];
	LLTableBuilder llTableBuilder(inputFileName);
	PrintTable(llTableBuilder.GetTable(), std::cout);
	return EXIT_SUCCESS;
}
