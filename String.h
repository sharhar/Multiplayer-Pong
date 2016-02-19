#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

struct String {
	char* text;
	int length;
};

class StringUtils{
public:

	static String newString(char* text, int length) {
		String temp = {text, length};
		return temp;
	}

	static String subString(String text, int start, int end) {
		int length = end-start;
		char newText[length];

		for(int i = 0;i < text.length;i++) {
			if(i >= start && i < end) {
				newText[i-start] = text.text[i];
			}
		}

		String result = {newText, length};
		return result;
	}

	static bool equals(String s1, String s2) {
		if(s1.length != s2.length) {
			return false;
		}

		for(int i =0;i < s1.length;i++) {
			if(s1.text[i] != s2.text[i]) {
				return false;
			}
		}

		return true;
	}

	static int find(String text, char thing) {
		int result = -1;
		
		for(int i = 0; i < text.length;i++) {
			if(text.text[i] == thing) {
				result = i;
				break;
			}
		}

		return result;
	}

	static String* split(String text, String regex) {
		String result[2];
		result[0] = newString((char*) "Hello world", 11);
		result[1] = newString((char*) "Hi", 2);
		return result;
	}

	static double parseDouble(String number) {
		return atof(number.text);
	}

	static float parseFloat(String number) {
		return (float) parseDouble(number);
	}

	static int parseInt(String number) {
		return (int) parseDouble(number);
	}
};

#endif