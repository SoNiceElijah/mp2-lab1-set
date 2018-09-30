// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
//fake changes

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw "Negative length";

	BitLen = len;

	//При условии, что TELEM 32 бита
	MemLen = (len + 31) >> 5;
	pMem = new TELEM[MemLen];

	//Обнуляем
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
		throw "Out of range";

	return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int index = GetMemIndex(n);
	pMem[index] = pMem[index] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int index = GetMemIndex(n);
	pMem[index] = pMem[index] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}

	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return  0;

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField tmp(*this);
	int min = bf.MemLen;

	if (BitLen < bf.BitLen)
	{
		tmp = bf;
		min = MemLen;
	}

	for (int i = 0; i < min; i++)
		tmp.pMem[i] = pMem[i] | bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField tmp(BitLen);
	int min = bf.MemLen;

	if (BitLen < bf.BitLen)
	{
		TBitField l(bf.BitLen);
		tmp = l;
		min = MemLen;
	}

	for (int i = 0; i < min; i++)
		tmp.pMem[i] = pMem[i] & bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);

	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~pMem[i];

	//Наверное можно проще
	TELEM mask = ~((~0) << (BitLen % 32));

	tmp.pMem[MemLen - 1] = tmp.pMem[MemLen - 1] & mask;

	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	return ostr;
}
