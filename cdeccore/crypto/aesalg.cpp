// -------------------------------------------------------------------------- //
//	文件名		：	AesAlg.cpp
//	创建者		：	Zhang Fan
//	创建时间	：	2010-4-28 21:51:55
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "aesalg.h"
// #include "CpuArch.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

#define xtime(x) ((((x) << 1) ^ (((x) & 0x80) != 0 ? 0x1B : 0)) & 0xFF)
#define Ui32(a0, a1, a2, a3) ((UInt32)(a0) | ((UInt32)(a1) << 8) | ((UInt32)(a2) << 16) | ((UInt32)(a3) << 24))
#define gb0(x) ( (x)          & 0xFF)
#define gb1(x) (((x) >> ( 8)) & 0xFF)
#define gb2(x) (((x) >> (16)) & 0xFF)
#define gb3(x) (((x) >> (24)) & 0xFF)

#define HT(i, x, s) (m_T + (x << 8))[gb ## x(s[(i + x) & 3])]
#define HT4(m, i, s, p) m[i] =	\
	HT(i, 0, s) ^				\
	HT(i, 1, s) ^				\
	HT(i, 2, s) ^				\
	HT(i, 3, s) ^ w[p + i]

/* such order (2031) in HT16 is for VC6/K8 speed optimization) */
#define HT16(m, s, p)			\
	HT4(m, 2, s, p);			\
	HT4(m, 0, s, p);			\
	HT4(m, 3, s, p);			\
	HT4(m, 1, s, p);
#define FT(i, x) m_Sbox[gb ## x(m[(i + x) & 3])]
#define FT4(i) dest[i] = Ui32(FT(i, 0), FT(i, 1), FT(i, 2), FT(i, 3)) ^ w[i];
#define HD(i, x, s) (m_D + (x << 8))[gb ## x(s[(i - x) & 3])]
#define HD4(m, i, s, p) m[i] =	\
	HD(i, 0, s) ^				\
	HD(i, 1, s) ^				\
	HD(i, 2, s) ^				\
	HD(i, 3, s) ^ w[p + i];

/* such order (0231) in HD16 is for VC6/K8 speed optimization) */
#define HD16(m, s, p)			\
	HD4(m, 0, s, p);			\
	HD4(m, 2, s, p);			\
	HD4(m, 3, s, p);			\
	HD4(m, 1, s, p);
#define FD(i, x) m_InvS[gb ## x(m[(i - x) & 3])]
#define FD4(i) dest[i] = Ui32(FD(i, 0), FD(i, 1), FD(i, 2), FD(i, 3)) ^ w[i];

// -------------------------------------------------------------------------

CAesAlg::CAesAlg()
{
	Byte	Sbox[256] = 
	{
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};
	memcpy((void*)m_Sbox, (void*)Sbox, 256);

	Byte Rcon[11] = { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
	memcpy((void*)m_Rcon, (void*)Rcon, 11);

	_AesGenTables();
}

void CAesAlg::SetKey(const Byte *key, unsigned keySize)
{
	_SetKeyEncode(m_AesEncodeCTX, key, keySize);
	_SetKeyDecode(key, keySize);
}

void CAesAlg::_SetKeyDecode(const Byte *key, unsigned keySize)
{
	unsigned i, num;
	UInt32 *w;
	_SetKeyEncode(m_AesDecodeCTX, key, keySize);
	num = m_AesDecodeCTX.numRounds2 * 8 - 4;
	w = m_AesDecodeCTX.rkey + 4;
	for (i = 0; i < num; i++)
	{
		UInt32 r = w[i];
		w[i] =
			m_D[        m_Sbox[gb0(r)]] ^
			m_D[0x100 + m_Sbox[gb1(r)]] ^
			m_D[0x200 + m_Sbox[gb2(r)]] ^
			m_D[0x300 + m_Sbox[gb3(r)]];
	}
}

void CAesAlg::Aes_Encode32(UInt32 *dest, const UInt32 *src)
{
	_AesEncode32(dest, src, m_AesEncodeCTX.rkey, m_AesEncodeCTX.numRounds2);
}

void CAesAlg::Aes_Decode32(UInt32 *dest, const UInt32 *src)
{
	_AesDecode32(dest, src, m_AesDecodeCTX.rkey, m_AesDecodeCTX.numRounds2);
}

void CAesAlg::_AesGenTables(void)
{
	unsigned i;
	for (i = 0; i < 256; i++)
		m_InvS[m_Sbox[i]] = (Byte)i;
	for (i = 0; i < 256; i++)
	{
		{
			UInt32 a1 = m_Sbox[i];
			UInt32 a2 = xtime(a1);
			UInt32 a3 = xtime(a1) ^ a1;
			m_T[        i] = Ui32(a2, a1, a1, a3);
			m_T[0x100 + i] = Ui32(a3, a2, a1, a1);
			m_T[0x200 + i] = Ui32(a1, a3, a2, a1);
			m_T[0x300 + i] = Ui32(a1, a1, a3, a2);
		}
		{
			UInt32 a1 = m_InvS[i];
			UInt32 a2 = xtime(a1);
			UInt32 a4 = xtime(a2);
			UInt32 a8 = xtime(a4);
			UInt32 a9 = a8 ^ a1;
			UInt32 aB = a8 ^ a2 ^ a1;
			UInt32 aD = a8 ^ a4 ^ a1;
			UInt32 aE = a8 ^ a4 ^ a2;
			m_D[        i] = Ui32(aE, a9, aD, aB);
			m_D[0x100 + i] = Ui32(aB, aE, a9, aD);
			m_D[0x200 + i] = Ui32(aD, aB, aE, a9);
			m_D[0x300 + i] = Ui32(a9, aD, aB, aE);
		}
	}
}

void CAesAlg::_SetKeyEncode(AesCTX &aes, const Byte *key, unsigned keySize)
{
	unsigned i, wSize;
	UInt32 *w;
	keySize /= 4;
	aes.numRounds2 = keySize / 2 + 3;

	wSize = (aes.numRounds2 * 2 + 1) * 4;
	w = aes.rkey;

	for (i = 0; i < keySize; i++, key += 4)
		w[i] = Ui32(key[0], key[1], key[2], key[3]);

	for (; i < wSize; i++)
	{
		UInt32 t = w[i - 1];
		unsigned rem = i % keySize;
		if (rem == 0)
			t = Ui32(m_Sbox[gb1(t)] ^ m_Rcon[i / keySize], m_Sbox[gb2(t)], m_Sbox[gb3(t)], m_Sbox[gb0(t)]);
		else if (keySize > 6 && rem == 4)
			t = Ui32(m_Sbox[gb0(t)], m_Sbox[gb1(t)], m_Sbox[gb2(t)], m_Sbox[gb3(t)]);
		w[i] = w[i - keySize] ^ t;
	}
}

void CAesAlg::_AesEncode32(UInt32 *dest, const UInt32 *src, const UInt32 *w, unsigned numRounds2)
{
	UInt32 s[4];
	UInt32 m[4];
	s[0] = src[0] ^ w[0];
	s[1] = src[1] ^ w[1];
	s[2] = src[2] ^ w[2];
	s[3] = src[3] ^ w[3];
	w += 4;
	for (;;)
	{
		HT16(m, s, 0);
		if (--numRounds2 == 0)
			break;
		HT16(s, m, 4);
		w += 8;
	}
	w += 4;
	FT4(0); FT4(1); FT4(2); FT4(3);
}

void CAesAlg::_AesDecode32(UInt32 *dest, const UInt32 *src, const UInt32 *w, unsigned numRounds2)
{
	UInt32 s[4];
	UInt32 m[4];
	w += numRounds2 * 8;
	s[0] = src[0] ^ w[0];
	s[1] = src[1] ^ w[1];
	s[2] = src[2] ^ w[2];
	s[3] = src[3] ^ w[3];
	for (;;)
	{
		w -= 8;
		HD16(m, s, 4);
		if (--numRounds2 == 0)
			break;
		HD16(s, m, 0);
	}
	FD4(0); FD4(1); FD4(2); FD4(3);
}

// -------------------------------------------------------------------------- //

AesTransform::AesTransform(CAesAlg* alg, CipherMode mode, ref<ByteArray> iv, bool fEncoder)
{
	m_alg = alg;
	m_mode = mode;

	if (mode != Cipher_ECB)
	{
		if (iv == NULL)
			cdec_throw(CryptoException(EC_CRYPT_InvalidIVSize));
		memcpy(m_iv, iv->GetBuffer().ptr(), 16);
	}

	if (mode == Cipher_ECB)
		m_e = fEncoder ? &AesTransform::f_EncodeECB : &AesTransform::f_DecodeECB;
	else if (mode == Cipher_CBC)
		m_e = fEncoder ? &AesTransform::f_EncodeCBC : &AesTransform::f_DecodeCBC;
}

AesTransform::~AesTransform()
{
	delete m_alg;
	m_alg = NULL;
}

void AesTransform::f_EncodeECB(const BYTE* src, BYTE* dest, int length)
{
	for (int off = 0; off < length; off += 16)
		m_alg->Aes_Encode32((UINT32*)(dest + off), (UINT32*)(src + off));
}

void AesTransform::f_DecodeECB(const BYTE* src, BYTE* dest, int length)
{
	for (int off = 0; off < length; off += 16)
		m_alg->Aes_Decode32((UINT32*)(dest + off), (UINT32*)(src + off));
}

inline void AesTransform::XOR16(BYTE* dest, const BYTE* src)
{
	UINT64 *pd = (UINT64*)dest;
	const UINT64 *ps = (const UINT64*)src;
	pd[0] ^= ps[0];
	pd[1] ^= ps[1];
}

inline void AesTransform::COPY16(BYTE* dest, const BYTE* src)
{
	UINT64 *pd = (UINT64*)dest;
	const UINT64 *ps = (const UINT64*)src;
	pd[0] = ps[0];
	pd[1] = ps[1];
}

void AesTransform::f_EncodeCBC(const BYTE* src, BYTE* dest, int length)
{
	for (int off = 0; off < length; off += 16)
	{
		XOR16(m_iv, src + off);
		m_alg->Aes_Encode32((UINT32*)(dest + off), (UINT32*)m_iv);
		COPY16(m_iv, dest + off);
	}
}

void AesTransform::f_DecodeCBC(const BYTE* src, BYTE* dest, int length)
{
	for (int off = 0; off < length; off += 16)
	{
		m_alg->Aes_Decode32((UINT32*)(dest + off), (UINT32*)(src + off));
		XOR16(dest + off, m_iv);
		COPY16(m_iv, src + off);
	}
}

// -------------------------------------------------------------------------- //

void AES::SetKey(ref<ByteArray> key)
{
	int len = key->Count();
	if (len == 16 || len == 24 || len == 32)
		m_key = key;
	else
		cdec_throw(CryptoException(EC_CRYPT_InvalidKeySize));
}

void AES::SetKey(const BYTE* key, int size)
{
	ref<ByteArray> key_a = gc_new<ByteArray>(key, size);
	SetKey(key_a);
}

void AES::SetIV(ref<ByteArray> iv)
{
	if (iv->Count() == 16)
		m_iv = iv;
	else
		cdec_throw(CryptoException(EC_CRYPT_InvalidIVSize));
}

void AES::SetIV(const BYTE* iv, int size)
{
	ref<ByteArray> iv_a = gc_new<ByteArray>(iv, size);
	SetIV(iv_a);
}

void AES::SetMode(CipherMode mode)
{
	switch (mode)
	{
	case Cipher_ECB:
	case Cipher_CBC:
		m_mode = mode;
		break;
	default:
		cdec_throw(CryptoException(EC_CRYPT_InvalidCipherMode));
	}
}

static CAesAlg* CreateAesAlg(ref<ByteArray> key)
{
	CAesAlg* p = new CAesAlg();
	pin_ptr<BYTE> pinKey = key->GetBuffer();
	p->SetKey(pinKey.ptr(), key->Count());
	return p;
}

ref<ICryptoTransform> AES::CreateEncryptor()
{
	CAesAlg* p = CreateAesAlg(m_key);
	return gc_new<AesTransform>(p, m_mode, m_iv, true);
}

ref<ICryptoTransform> AES::CreateDecryptor()
{
	CAesAlg* p = CreateAesAlg(m_key);
	return gc_new<AesTransform>(p, m_mode, m_iv, false);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
