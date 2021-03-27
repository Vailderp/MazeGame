#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
const int bitness = 8;
namespace vb
{

	//class vaibin
	//{
	//public:

	class NUMBER_SYSTEM
	{
	public:

		std::string chars = "0123456789ABCD;=";
		std::string closecharbin = "1110";
		int len = 16;
		int bitness = 4;

	};

	NUMBER_SYSTEM main_ns;

	int BinToDec(std::string bin)
	{
		int binlen = bin.length() - 1;
		int num = 0;
		for (size_t i = 0; i < bin.length(); i++)
		{
			if (bin[i] == '0')
			{
				num += 0;
			}
			else if (bin[i] == '1')
			{
				num += int(bin[i] - '0') * pow(2, binlen - i);
			}
		}
		return num;
	}

	std::string DecToBin(unsigned dec)
	{
		std::string output = "";
		int len = 0;
		while (dec >> len > 0)
		{
			output += std::to_string((dec >> len) % 2);
			len++;
		}
		reverse(output.begin(), output.end());
		return output;
	}

	std::string DecToAnyNumberSystem(unsigned dec, unsigned numsys)
	{
		std::string output = "";
		while (0 < dec)
		{
			output += std::to_string(dec % numsys);
			dec = dec / numsys;
		}
		reverse(output.begin(), output.end());
		return output;
	}

	std::string ReadFile(std::string path)
	{
		std::fstream file;
		file.open(path);
		std::string time_t;
		std::string filetxt = "";
		while (getline(file, time_t)) {
			filetxt += time_t;
		}
		file.close();
		return filetxt;
	}

	void WriteFile(std::string input, std::string path)
	{
		std::fstream file;
		file.open(path);
		file << input;
		file.close();
	}

	std::string GetBinChar(char c)
	{
		int k = 128;
		std::string bin = "";
		while (k > 0) {
			bin += std::to_string(bool(c & k));
			k = k >> 1;
		};
		return bin;
	}

	std::string GetBinText(std::string inputtxt)
	{
		std::string bin_chars = "";
		std::string timermaptrext;
		for (size_t i = 0; i < inputtxt.length(); i++)
		{
			bin_chars += GetBinChar(inputtxt[i]);
		}
		return bin_chars;
	}

	std::string GetBinFile(std::string path)
	{
		std::string bin_chars = "";
		std::fstream file;
		file.open(path);
		std::string timermaptrext;
		std::string txt = "";
		while (getline(file, timermaptrext)) {
			txt += timermaptrext;
		}
		for (size_t i = 0; i < txt.length(); i++)
		{
			bin_chars += GetBinChar(txt[i]);
		}
		return bin_chars;
		file.close();
	}

	void SetBinFile(std::string bin, std::string path)
	{
		std::string output;
		std::fstream file;
		file.open(path);
		if (bin.length() % bitness == 0)
		{
			for (size_t i = 0; i < bin.length() / bitness; i++)
			{
				std::string tt = "";
				for (size_t l = 0; l < bitness; l++)
				{
					tt += bin[i * bitness + l];
				}
				char charset(BinToDec(tt));
				output += charset;
			}
		}
		else
		{
			std::cout << std::endl << "ERROR: BIN % 8 != 0";
		}
		file << output;
		file.close();
	}

	std::string SetBinString(std::string bin)
	{
		std::string output;
		if (bin.length() % bitness == 0)
		{
			for (size_t i = 0; i < bin.length() / bitness; i++)
			{
				std::string tt = "";
				for (size_t l = 0; l < bitness; l++)
				{
					tt += bin[i * bitness + l];
				}
				char charset(BinToDec(tt));
				output += charset;
			}
		}
		else
		{
			std::cout << std::endl << "ERROR: BIN % 8 != 0";
		}
		return output;
	}

	std::string CreateNSBin(std::string input, NUMBER_SYSTEM ns)
	{
		std::string output = "";
		for (size_t i = 0; i < input.length(); i++)
		{
			std::string tt = "";
			tt = DecToBin(ns.chars.find_first_of(input[i]));
			while (tt.length() < 4)
			{
				tt = "0" + tt;
			}
			output += tt;
		}
		if (input.length() % (bitness / ns.bitness) != 0)
		{
			output += ns.closecharbin;
		}
		return output;
	}

	std::string GetNSBin(std::string input, NUMBER_SYSTEM ns)
	{
		std::string output = "";
		for (size_t i = 0; i < input.length(); i += ns.bitness)
		{
			std::string bin = "";
			for (size_t l = 0; l < ns.bitness; l++)
			{
				bin += input[i + l];
			}
			output += ns.chars[BinToDec(bin)];
		}
		return output;
	}

	std::map<std::string, std::string> ReadConf(std::string input)
	{
		std::map <std::string, std::string> output;
		std::string t = "";
		std::string tt = "";
		for (size_t i = 0; i < input.length(); i++)
		{
			if (input[i] != ' ')
			{
				while (input[i] != '=')
				{
					if (input[i] != ' ')
					{
						t += input[i];
					}
					i++;
				}
				i++;
				while (input[i] != ';')
				{
					if (input[i] != ' ')
					{
						tt += input[i];
					}
					i++;
				}
				output[t] = tt;
				t = "";
				tt = "";
			}
		}
		return output;
	}

	std::map<std::string, std::string> ReadConfFile(std::string path)
	{
		std::fstream file;
		file.open(path);
		std::string time_t;
		std::string filetxt = "";
		while (std::getline(file, time_t)) {
			filetxt += time_t;
		}
		std::map <std::string, std::string> output;
		std::string t = "";
		std::string tt = "";
		for (size_t i = 0; i < filetxt.length(); i++)
		{
			if (filetxt[i] != ' ')
			{
				while (filetxt[i] != '=')
				{
					if (filetxt[i] != ' ')
					{
						t += filetxt[i];
					}
					i++;
				}
				i++;
				while (filetxt[i] != ';')
				{
					if (filetxt[i] != ' ')
					{
						tt += filetxt[i];
					}
					i++;
				}
				output[t] = tt;
				t = "";
				tt = "";
			}
		}
		file.close();
		return output;
	}

	std::string WriteConf(std::map<std::string, std::string> input)
	{
		std::string output = "";
		for (auto& item : input)
		{
			output += item.first + "=" + item.second + ";";
		}
		return output;
	}

	void WriteConfFile(std::map<std::string, std::string> input, std::string path)
	{
		std::fstream file;
		file.open(path);
		std::string output = "";
		for (auto& item : input)
		{
			output += item.first + "=" + item.second + ";";
		}
		file << output;
		file.close();
	}

	std::string BeautiferConf(std::string input)
	{
		std::string t = "";
		std::string tt = "";
		std::string output = "";
		for (size_t i = 0; i < input.length(); i++)
		{
			if (input[i] != ' ')
			{
				while (input[i] != '=')
				{
					if (input[i] != ' ')
					{
						t += input[i];
					}
					i++;
				}
				output += t;
				output += " = ";
				i++;
				while (input[i] != ';')
				{
					if (input[i] != ' ')
					{
						tt += input[i];
					}
					i++;
				}
				output += tt;
				output += ";\n";
				t = "";
				tt = "";
			}
		}
		return output;
	}

	std::vector<std::string> ReadVectorConfFile(std::string path)
	{
		std::fstream file;
		file.open(path);
		std::string time_t;
		std::string filetxt = "";
		while (std::getline(file, time_t)) {
			filetxt += time_t;
		}
		std::vector<std::string> output;
		std::string t = "";
		std::string tt = "";
		for (size_t i = 0; i < filetxt.length(); i++)
		{
			if (filetxt[i] != ' ')
			{
				while (filetxt[i] != ';')
				{
					if (filetxt[i] != ' ')
					{
						tt += filetxt[i];
					}
					i++;
				}
				output.push_back(tt);
				t = "";
				tt = "";
			}
		}
		file.close();
		return output;
	}
	
	std::vector<char> readBigFile(std::string path)
	{
		std::fstream file;
		file.open(path);
		std::string time_t;
		std::vector<char> output;
		while (std::getline(file, time_t)) {
			for (unsigned int i = 0; i < time_t.size(); i++)
			{
				output.push_back(time_t[i]);
			}
		}
		file.close();
		return output;
	}

	std::vector<std::string> getFileLexiconVector(std::string path)
	{
		std::fstream file;
		file.open(path);
		std::string line;
		std::string line_data;
		std::vector<std::string> output;
		while (std::getline(file, line)) {
			for (unsigned int i = 0; i < line.size(); i++)
			{
				while (line[i] != ' ')
				{
					line_data += line[i];
					i++;
				}
				output.push_back(line_data);
				line_data = "";
			}
		}
		return output;
	}

	/*inline map<string, string> parse_XML(string input, unsigned int begin_input, unsigned int end_input)
	{
		map<string, string> xml;
		unsigned in_len = end_input - begin_input;
		for  (unsigned int i = 0; i < in_len; i++)
		{

		}
		return xml;
	}

	map<string, string> VAI_XML_READ(string input)
	{
		unsigned int in_len = input.length();
		string pre_str = "";
		string ps = "";
		for (unsigned int i = 0; i < in_len; i++)
		{
			if (input[i] == '<')
			{
				i++;
				while (input[i] != '>')
				{
					pre_str += input[i];
				}
				i++;
				while (input[i] != '<')
				{

				}

			}
		}
	}*/
}