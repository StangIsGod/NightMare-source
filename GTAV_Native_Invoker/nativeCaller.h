/* kurt2467*/
bool isNumericChar(char x)
{
	return (x >= '0' && x <= '9') ? true : false;
}

int StoI(char *str)
{
	if (*str == NULL)
		return 0;

	int res = 0,
		sign = 1,
		i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; str[i] != '\0'; ++i)
	{
		if (isNumericChar(str[i]) == false)
			return 0;
		res = res * 10 + str[i] - '0';
	}
	return sign*res;
}
char *ItoS(int num)
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%i", num);
	return buf;
}
char* FtoS(float input)
{
	char returnvalue[64];
	int wholenumber = (int)input;
	input -= wholenumber;
	input *= 100;
	sprintf(returnvalue, "%d.%d", wholenumber, (int)input);
	return returnvalue;
}
namespace PS3
{
	int intArray[100];
	char byteArray[100];
	unsigned int byteArray2;
	int* ReadInt(int address, int length)
	{
		for (int i = 0; i < length; i++)
		{
			intArray[i] = *(int*)(address + (i * 0x04));
		}
		return intArray;
	}
	int ReadInt16(int address)
	{
		for (int i = 0; i < 2; i++)
		{
			byteArray[i] = *(char*)(address + (i));
		}

		char *byteArray3 = strcat(&byteArray[1], &byteArray[2]);
		byteArray2 = StoI(byteArray3);
		return byteArray2;
	}
	void WriteInt(int address, int* input, int length)
	{
		for (int i = 0; i < length; i++)
		{
			*(int*)(intArray + (i * 0x04)) = input[i];
		}
	}
	void WriteFloat(int address, float input)
	{
		*(float*)(address) = input;
	}
	int32_t write_process(uint64_t ea, const void * data, uint32_t size)
	{
		system_call_4(905, (uint64_t)sys_process_getpid(), ea, size, (uint64_t)data);
		return_to_user_prog(int32_t);
	}
	void WriteBytes(int address, char* input, int length)
	{

		for (int i = 0; i < length; i++)
		{
			*(char*)(address + (i)) = input[i];
		}

	}
	char* ReadBytes(int address, int length)
	{
		for (int i = 0; i < length; i++)
		{
			byteArray[i] = *(char*)(address + (i));
		}
		return byteArray;
	}
	void WriteString(int address, char* string)
	{
		int FreeMem = 0x1D00000;
		int strlength = std::strlen(string);
		char* strpointer = *(char**)FreeMem = string;
		char* StrBytes = PS3::ReadBytes(*(int*)FreeMem, strlength);
		PS3::WriteBytes(address, StrBytes, strlength);
		*((char*)address + strlength) = '\0';
	}
	char returnRead[100];
	char* ReadString(int address)
	{
		memset(&returnRead[0], 0, sizeof(returnRead));
		int strlength = 100;
		char* StrBytes = ReadBytes(address, strlength);
		for (int i = 0; i < strlength; i++)
		{
			if (StrBytes[i] != 0x00)
				returnRead[i] = StrBytes[i];
			else
				break;
		}
		return returnRead;
	}
	char returnR[100];
	const char *ReadString1(int address)
	{
		memset(&returnRead[0], 0, sizeof(returnR));
		int strlength = 100;
		const char *StrBytes = ReadBytes(address, strlength);
		for (int i = 0; i < strlength; i++)
		{
			if (StrBytes[i] != 0x00)
				returnRead[i] = StrBytes[i];
			else
				break;
		}
		return returnR;
	}
	void WriteInt32(int Address, int Input)
	{
		*(int*)Address = Input;
	}
	int ReadInt32(int Address)
	{
		return *(int*)Address;
	}
	void WriteUInt32(int Address, unsigned int Input)
	{
		*(unsigned int*)Address = Input;
	}
	unsigned int ReadUInt32(int Address)
	{
		return *(unsigned int*)Address;
	}
	float floatArray[100];
	float* ReadFloats(int address, int length)
	{
		for (int i = 0; i < length; i++)
		{
			floatArray[i] = *(float*)(address + (i * 0x04));
		}
		return floatArray;
	}

}

typedef long long __int64;

__int64 GetPlayerIndex_Stub(int TimeId) {
	__asm("li %r3, 0x3");
}
struct Native_s
{
	Native_s*	LastNativesTable;
	unsigned int 	NativeFunctions[7];
	unsigned int 	NativeCount;
	unsigned int 	NativeHashes[7];
};

struct NativeArg_s
{
	int* ReturnValue;
	unsigned int ArgCount;
	int* ArgValues;
	unsigned int ReadCount;
};

extern Native_s** g_Natives;
static NativeArg_s* NativeArg = (NativeArg_s*)0x10060000; //Open space addr

void ResetArgs()
{
	NativeArg->ArgCount = 0;
	NativeArg->ReadCount = 0;
	NativeArg->ArgValues = (int*)0x10050000;
	NativeArg->ReturnValue = NativeArg->ArgValues;
}
template<typename T>
void PushArg(T Value)
{
	*(T*)&NativeArg->ArgValues[NativeArg->ArgCount] = Value;
	NativeArg->ArgCount++;
}
template<typename R>
R GetReturn()
{
	R* Result;
	Result = (R*)&NativeArg->ReturnValue[0];
	return *(R*)Result;
}
void CallHash(unsigned int hash, NativeArg_s* pArg)
{
	Native_s* Natives = g_Natives[hash & 0xFF];
	while (Natives != 0)
	{
		for (unsigned int i = 0; i < Natives->NativeCount; i++)
		{
			if (Natives->NativeHashes[i] == hash)
			{
				((void(*)(NativeArg_s*))Natives->NativeFunctions[i])(pArg);
				return;
			}
		}
		Natives = Natives->LastNativesTable;
	}
}

template<typename T>
T invoke(unsigned int NativeHash)
{
	ResetArgs();
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1>
T invoke(unsigned int NativeHash, P1 p0)
{
	ResetArgs();
	PushArg(p0);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2>
T invoke(unsigned int NativeHash, P1 p0, P2 p1)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	PushArg(p20);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	PushArg(p20);
	PushArg(p21);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	PushArg(p20);
	PushArg(p21);
	PushArg(p22);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23, typename P24>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22, P24 p23)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	PushArg(p20);
	PushArg(p21);
	PushArg(p22);
	PushArg(p23);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23, typename P24, typename P25>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22, P24 p23, P25 p24)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	PushArg(p20);
	PushArg(p21);
	PushArg(p22);
	PushArg(p23);
	PushArg(p24);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23, typename P24, typename P25, typename P26>
T invoke(unsigned int NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22, P24 p23, P25 p24, P26 p25)
{
	ResetArgs();
	PushArg(p0);
	PushArg(p1);
	PushArg(p2);
	PushArg(p3);
	PushArg(p4);
	PushArg(p5);
	PushArg(p6);
	PushArg(p7);
	PushArg(p8);
	PushArg(p9);
	PushArg(p10);
	PushArg(p11);
	PushArg(p12);
	PushArg(p13);
	PushArg(p14);
	PushArg(p15);
	PushArg(p16);
	PushArg(p17);
	PushArg(p18);
	PushArg(p19);
	PushArg(p20);
	PushArg(p21);
	PushArg(p22);
	PushArg(p23);
	PushArg(p24);
	PushArg(p25);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

