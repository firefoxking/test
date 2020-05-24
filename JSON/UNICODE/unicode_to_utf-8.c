#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#define INIT_SIZE 256
struct buff{
	char *stk;
	int SIZE, top;
}buff;
char *push(int len)
{
	if(buff.top + len >= buff.SIZE)
	{
		if(buff.SIZE == 0)	buff.SIZE = INIT_SIZE;
		while(buff.top + len >= buff.SIZE)
		{
			buff.SIZE += buff.SIZE >> 1;
		}
		buff.stk = (char *)realloc(buff.stk, buff.SIZE);
	}
	char *ret = buff.stk + buff.top;
	buff.top += len;
	return ret;
}
char *pop(int len)
{
	assert(buff.top >= len);
	return buff.stk + (buff.top -= len);
}
void put(char c)
{
	*push(sizeof(c)) = c;
}
char *parse_hex4(char *p, unsigned *u)
{
	*u = 0;
	printf("parse_hex4 unsigned has %d byte\n", sizeof(unsigned));
	for(int i = 0; i < 4; ++i)
	{
		char c = *p++;
		(*u) <<= 4;
		if(c>='0' && c<='9')		*u += c - '0';
		else if(c>='a' && c<='f')	*u += c - 'a';
		else if(c>='A' && c<='F')	*u += c - 'A';
		else return NULL;
	}
	return p;
}
void encode_utf8(unsigned u)
{
	if(u <= 0x7f)
		put(u & 0xff);
	else if(u <= 0x7ff)
	{
		put(0xc0 | ((u >> 6) & 0xff));
		put(0x80 | (u & 0x3f));
	}
	else if (u <= 0xFFFF) {
       		put(0xE0 | ((u >> 12) & 0xFF));
        	put(0x80 | ((u >>  6) & 0x3F));
        	put(0x80 | ( u        & 0x3F));
    	}
    	else {
        	assert(u <= 0x10FFFF);
        	put(0xF0 | ((u >> 18) & 0xFF));
        	put(0x80 | ((u >> 12) & 0x3F));
        	put(0x80 | ((u >>  6) & 0x3F));
        	put(0x80 | ( u        & 0x3F));
	}
}
bool parse_string(char *str)
{
	char *p = str;
	size_t head = buff.top, len;
	unsigned u, u2;
	if(p == NULL || *p != '\"')	return false;
	++p;
	while(true)
	{
		char ch = *p++;
		switch(ch){
			case '\"':
				len = buff.top-head;
				s = (char *)malloc(len);
				memcpy(s, pop(len), len);
				return true;
			case '\\':
				switch(*p++){
					case '\"':	put('\"'); break;
					case '\\':	put('\\'); break;
					case '/':	put('/');  break;
					case 'b':	put('\b';  break;
					case 'f':	put('\f'); break;
					case 'n':	put('\n'); break;
					case 'r':	put('\r'); break;
					case 't':	put('\t'); break;
					case 'u':
						if(!(p = parse_hex4(p, &u))) return false;
						if(u>=0xD800 && u<=0xDBFF){
							if(*p++ != '\\') return false;
							if(*p++ != 'u') return false;
							if(!(p = parse_hex4(p, &u2))) return false;
							if(u2 < 0xDC00 || u2>0xDFFF) return false;
							u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
							}
							break;
					default:	return false;
					}
			case '\0': return false;
			default:
				if((unsigned char)ch < 0x20)
					return false;
				put(ch);
		}
	}
}

int main(){
	buff.stk = s = NULL;
	buff.SIZE = buff.top = 0;
	parse_string("\"hello\"");
	return 0;
}
