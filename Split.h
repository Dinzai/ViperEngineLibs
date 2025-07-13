/*

MIT Licence
Author -> Brett Rogers
Reference -> https://www.youtube.com/watch?v=jwNRV9Al83Y&t=225s


String Splitting Utility

Map building Use case(psudo code, that code not included) ->

split a txt file that has 0's, 1's and commas
remove the commas, keep the 0's and 1's
1's = wall/floor
0's = empty

How to Use?

Run Do(file.txt, ',');     <------ run this function, split by the ignore character

if data needs to be type int run

std::vector<int> numList = ConvertToInt(goal);


*/

#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

namespace StringSplit
{
	struct HelperFunctions
	{


		HelperFunctions()
		{
		}

		~HelperFunctions()
		{
		}

	private:
		std::vector<std::string> AddData(std::string fileLocation)
		{
			std::vector<std::string> data;

			std::ifstream in(fileLocation);
			std::string w;

			if (in.is_open())
			{

				while (std::getline(in, w))
				{
					data.push_back(w);
				}
				in.close();
			}
			else
			{
				std::cout << "failed";
			}
			return data;
		}

		std::vector<std::string> Split(std::string input, char splitChar)
		{

			std::vector<std::string> output;

			bool hasHitSplit = false;

			std::string tempString;

			for (int i = 0; i < input.size(); i++)
			{

				if (input.at(i) != splitChar && !hasHitSplit)
				{
					tempString += input.at(i);
				}

				else if (input.at(i) == splitChar)
				{
					hasHitSplit = true;
				}

				if (hasHitSplit)
				{
					output.push_back(tempString);
					tempString = "";
					hasHitSplit = false;
				}

			}

			return output;

		}

	public:

		std::vector<int> ConverToInt(std::vector<std::string> stringArray)
		{
			std::vector<int> trueData;

			for (size_t i = 0; i < stringArray.size(); i++)
			{
				int tempNum = std::stoi(stringArray[i]);
				trueData.push_back(tempNum);
			}
			return trueData;

		}

		void Do(std::string fileLocation, char splitChar)
		{

			std::vector<std::string> in = AddData(fileLocation);
			std::vector<std::string> out;

			for (int i = 0; i < in.size(); i++)
			{
				std::string chunk = in.at(i);
				out = Split(chunk, splitChar);

				for (int j = 0; j < out.size(); j++)
				{

					goal.push_back(out.at(j));
				}

			}


		}

		std::vector<std::string> goal;
	};
}