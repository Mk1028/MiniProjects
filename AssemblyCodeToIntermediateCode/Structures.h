#ifndef _Structures_h
#define _Strctures_h

struct SymbolTable
{
	char Name[10];
	char Type[10];
	int Address;
	int Size;
};

typedef SymbolTable ST;

struct LabelTable
{
	char BlockName[5];
	int Address;
};

typedef LabelTable LT;

struct IntermediateLang
{
	int Opcode;
	int par[4];
};

typedef IntermediateLang IL;

#endif // !h_Structures_h

