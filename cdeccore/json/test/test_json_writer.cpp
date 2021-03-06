#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

using namespace cdec::json_express;

class TestJsonWriter : public UnitTestSuite
{
	UNITTEST_SUITE(TestJsonWriter)

		UNITTEST_METHOD(TestWriteValues)

		UNITTEST_METHOD(TestWriteDictionary)
		UNITTEST_METHOD(TestWriteDictionaryFormat)
		UNITTEST_METHOD(TestWriteList)		
		UNITTEST_METHOD(TestWriteListFormat)

		UNITTEST_METHOD(TestWriteDictionaryUnderDictionary)
		UNITTEST_METHOD(TestWriteDictionaryUnderDictionaryFormat)
		UNITTEST_METHOD(TestWriteListUnderDictionary)
		UNITTEST_METHOD(TestWriteListUnderDictionaryFormat)

		UNITTEST_METHOD(TestWriteDictionaryUnderList)
		UNITTEST_METHOD(TestWriteDictionaryUnderListFormat)
		UNITTEST_METHOD(TestWriteListUnderList)
		UNITTEST_METHOD(TestWriteListUnderListFormat)

		UNITTEST_METHOD(TestComplextSample)

		UNITTEST_METHOD_EXCEPTION(ErrorMultiRoot)
		UNITTEST_METHOD_EXCEPTION(ErrorRootNodeHasName)
		UNITTEST_METHOD_EXCEPTION(ErrorListChildHasName)
		UNITTEST_METHOD_EXCEPTION(ErrorDictionaryChildWithoutName)
		UNITTEST_METHOD_EXCEPTION(ErrorDictionaryNotClosed)
		UNITTEST_METHOD_EXCEPTION(ErrorCloseNonexistDictionary)
		UNITTEST_METHOD_EXCEPTION(ErrorCloseNonexistList)
		UNITTEST_METHOD_EXCEPTION(ErrorCloseDictionaryByList)
		UNITTEST_METHOD_EXCEPTION(ErrorCloseListByDictionary)

		UNITTEST_METHOD(TestJsonWriteExpression)
		UNITTEST_METHOD(TestJsonWriteExpressionFormat)
		UNITTEST_METHOD(TestJsonWriteExpressionNested)

		UNITTEST_METHOD(TestJsonWriteReopenDictionary)
		UNITTEST_METHOD(TestJsonWriteReopenList)
		UNITTEST_METHOD(TestJsonWriteReopenComplex)

	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void TestWriteValues()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->WriteString(NULL, __X("abc"));
		UNITTEST_ASSERT(wr->GetString() == __X("\"abc\""));

		wr->Reset();
		wr->WriteInt(NULL, 1234567890123456789L);
		UNITTEST_ASSERT(wr->GetString() == __X("1234567890123456789"));

		wr->Reset();
		wr->WriteBool(NULL, true);
		UNITTEST_ASSERT(wr->GetString() == __X("true"));

		wr->Reset();
		wr->WriteBool(NULL, false);
		UNITTEST_ASSERT(wr->GetString() == __X("false"));

		wr->Reset();
		wr->WriteNull(NULL);
		UNITTEST_ASSERT(wr->GetString() == __X("null"));

		// With format
		wr->Reset();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->WriteString(NULL, __X("abc"));
		UNITTEST_ASSERT(wr->GetString() == __X("\"abc\""));
	}

	void TestWriteDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"name\":\"abc\"}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->WriteInt(__X("value"), 123);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"name\":\"abc\",\"value\":123}"));
	}

	void TestWriteDictionaryFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"name\":\"abc\"\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->WriteInt(__X("value"), 123);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"name\":\"abc\",\n \"value\":123\n}"));
	}

	void TestWriteList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\"abc\"]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\"abc\",123]"));
	}

	void TestWriteListFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginList(NULL);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n \"abc\"\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n \"abc\",\n 123\n]"));
	}

	void TestWriteDictionaryUnderDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"dict\":{}}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"dict\":{\"key\":123}}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"dict\":{\"key\":123,\"value\":456}}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"dict\":{\"key\":123},\"value\":456}"));
	}

	void TestWriteDictionaryUnderDictionaryFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"dict\":{\n }\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"dict\":{\n  \"key\":123\n }\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"dict\":{\n  \"key\":123,\n  \"value\":456\n }\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"dict\":{\n  \"key\":123\n },\n \"value\":456\n}"));
	}

	void TestWriteListUnderDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"list\":[]}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"list\":[123]}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"list\":[123,456]}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\"list\":[123],\"value\":456}"));
	}

	void TestWriteListUnderDictionaryFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"list\":[\n ]\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"list\":[\n  123\n ]\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"list\":[\n  123,\n  456\n ]\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->GetString() == __X("{\n \"list\":[\n  123\n ],\n \"value\":456\n}"));
	}

	void TestWriteDictionaryUnderList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[{}]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[{\"key\":123}]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[{\"key\":123,\"value\":456}]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[{\"key\":123},456]"));
	}

	void TestWriteDictionaryUnderListFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n {\n }\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n {\n  \"key\":123\n }\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n {\n  \"key\":123,\n  \"value\":456\n }\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n {\n  \"key\":123\n },\n 456\n]"));
	}

	void TestWriteListUnderList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[[]]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[[123]]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[[123,456]]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[[123],456]"));
	}

	void TestWriteListUnderListFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n [\n ]\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n [\n  123\n ]\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n [\n  123,\n  456\n ]\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->GetString() == __X("[\n [\n  123\n ],\n 456\n]"));
	}

	void TestComplextSample()
	{
		stringx s = stringx() +
			__X("{\n") +
			__X("  \"return\":{\n") +
			__X("    \"u\":[\n") +
			__X("      {\n") +
			__X("        \"u\":\"http://118.144.67.133/ufa/\",\n") +
			__X("        \"t\":2\n") +
			__X("      },\n") +
			__X("      {\n") +
			__X("        \"u\":\"http://61.189.5.13/ufa/\",\n") +
			__X("        \"t\":1\n") +
			__X("      }\n") +
			__X("    ],\n") +
			__X("    \"p\":\"2\"\n") +
			__X("  },\n") +
			__X("  \"code\":\"Ok\"\n") +
			__X("}");

		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X("  ");
		wr->NewLineChars = __X("\n");

		wr->BeginDictionary(NULL);
			wr->BeginDictionary(__X("return"));
			wr->BeginList(__X("u"));
				wr->BeginDictionary(NULL);
				wr->WriteString(__X("u"), __X("http://118.144.67.133/ufa/"));
				wr->WriteInt(__X("t"), 2);
				wr->EndDictionary();
				wr->BeginDictionary(NULL);
				wr->WriteString(__X("u"), __X("http://61.189.5.13/ufa/"));
				wr->WriteInt(__X("t"), 1);
				wr->EndDictionary();
			wr->EndList();
			wr->WriteString(__X("p"), __X("2"));
			wr->EndDictionary();			  
			wr->WriteString(__X("code"), __X("Ok"));
		wr->EndDictionary();

		stringx t = wr->GetString();
		UNITTEST_ASSERT(s == t);
	}

	void ErrorMultiRoot()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
	}

	void ErrorRootNodeHasName()
	{
		// A root value node should not have a name
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->WriteString(__X("name"), __X("abc"));
	}

	void ErrorListChildHasName()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->WriteString(__X("name"), __X("abc"));
	}

	void ErrorDictionaryChildWithoutName()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->WriteString(NULL, __X("abc"));
	}

	void ErrorDictionaryNotClosed()
	{
		// A dictionary is not closed before completing
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->GetString();
	}
	
	void ErrorCloseNonexistDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		wr->EndDictionary();
	}
	
	void ErrorCloseNonexistList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->EndList();
		wr->EndList();
	}
	
	void ErrorCloseDictionaryByList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->EndList();
	}

	void ErrorCloseListByDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->EndDictionary();
	}

	void TestJsonWriteExpression()
	{
		JE expr = JE::New() + __X("abc");
		UNITTEST_ASSERT(expr.GetString() == __X("\"abc\""));

		expr = JE::New() + 123;
		UNITTEST_ASSERT(expr.GetString() == __X("123"));

		expr = JE::New() + JE::Bool(true);
		UNITTEST_ASSERT(expr.GetString() == __X("true"));

		expr = JE::New() + JE::None();
		UNITTEST_ASSERT(expr.GetString() == __X("null"));

		expr = JE::New() +
			JE::Dict() +
				JE::Pair(__X("name"), __X("abc")) +
				JE::Pair(__X("value"), 123) +
				JE::PairBool(__X("switch"), false) +
				JE::None(__X("addon")) +
			JE::EDict();
		UNITTEST_ASSERT(expr.GetString() == __X("{\"name\":\"abc\",\"value\":123,\"switch\":false,\"addon\":null}"));

		expr = JE::New() +
			JE::List() +
				__X("abc") +
				123 +
				JE::Bool(true) +
				JE::None() +
			JE::EList();
		UNITTEST_ASSERT(expr.GetString() == __X("[\"abc\",123,true,null]"));

		expr = JE::New() +
			JE::Dict() +
				JE::Pair(__X("result"), __X("ok")) +
				JE::List(__X("items")) +
					JE::Dict() +
						JE::Pair(__X("name"), __X("abc")) +
						JE::Pair(__X("value"), 123) +
					JE::EDict() +
					JE::Dict() +
						JE::Pair(__X("name"), __X("def")) +
						JE::Pair(__X("value"), 456) +
					JE::EDict() +
				JE::EList() +
				JE::List(__X("weight")) + 
					__X("nil") +
					180 +
				JE::EList() +
			JE::EDict();
		stringx s = stringx() +
			__X("{") +
				__X("\"result\":\"ok\",") +
				__X("\"items\":[") +
					__X("{\"name\":\"abc\",\"value\":123},") +
					__X("{\"name\":\"def\",\"value\":456}") +
				__X("],") +
				__X("\"weight\":[\"nil\",180]") +
			__X("}");
		stringx t = expr.GetString();
		UNITTEST_ASSERT(s == t);
	}

	void TestJsonWriteExpressionFormat()
	{
		JE expr = JE::New() +
			JE::Dict() +
				JE::Pair(__X("name"), __X("abc")) +
				JE::Pair(__X("value"), 123) +
			JE::EDict();
		ref<JsonExpressFormater> jsf = gc_new<JsonExpressFormater>();
		jsf->IndentChars = __X(" ");
		jsf->NewLineChars = __X("\n");
		UNITTEST_ASSERT(expr.GetString(jsf) == __X("{\n \"name\":\"abc\",\n \"value\":123\n}"));
	}

	void TestJsonWriteExpressionNested()
	{
		// A dictionary sub-expression
		JE sub = JE::New() +
			JE::Dict() +
				JE::Pair(__X("name"), __X("abc")) +
				JE::Pair(__X("value"), 123) +
			JE::EDict();
		JE expr = JE::New() + sub;
		UNITTEST_ASSERT(expr.GetString() == __X("{\"name\":\"abc\",\"value\":123}"));

		// A dictionary sub-expression under a list
		expr = JE::New() +
			JE::List() +
				1 + 
				sub + 
				2 +
			JE::EList();
		UNITTEST_ASSERT(expr.GetString() == __X("[1,{\"name\":\"abc\",\"value\":123},2]"));

		// A dictionary sub-expression under a dictionary
		expr = JE::New() +
			JE::Dict() +
				JE::Pair(__X("result"), __X("ok")) +
				JE::Pair(__X("body"), sub) +
				JE::Pair(__X("extra"), 0) +
			JE::EDict();
		UNITTEST_ASSERT(expr.GetString() == __X("{\"result\":\"ok\",\"body\":{\"name\":\"abc\",\"value\":123},\"extra\":0}"));

		// A list sub-expression
		sub = JE::New() +
			JE::List() +
				__X("abc") +
				123 +
			JE::EList();
		expr = JE::New() + sub;
		UNITTEST_ASSERT(expr.GetString() == __X("[\"abc\",123]"));

		// A list sub-expression under a list
		expr = JE::New() +
			JE::List() + 
				1 + 
				sub + 
				2 +
			JE::EList();
		UNITTEST_ASSERT(expr.GetString() == __X("[1,[\"abc\",123],2]"));

		// A list sub-expression under a dictionary
		expr = JE::New() +
			JE::Dict() +
				JE::Pair(__X("result"), __X("ok")) +
				JE::Pair(__X("body"), sub) +
				JE::Pair(__X("extra"), 0) +
			JE::EDict();
		UNITTEST_ASSERT(expr.GetString() == __X("{\"result\":\"ok\",\"body\":[\"abc\",123],\"extra\":0}"));
	}

	void TestJsonWriteReopenDictionary()
	{
		// Original JE
		JE expr = JE::New() + 
			JE::Dict() + 
				JE::Pair(__X("a"), 1) +
			JE::EDict();
		ref<JsonNode> root = expr.Complete();	// the root dictionary node to be re-opened

		// Method 1, manually add a new JsonNode
		ref<JsonNode> node = JsonNode::NewIntNode(2);
		node->SetName(__X("b"));
		root->AddDictionaryItem(node);
		UNITTEST_ASSERT(JE(root).GetString() == __X("{\"a\":1,\"b\":2}"));

		// Method 2, using JsonWriter re-open function
		ref<JsonWriter> wr = gc_new<JsonWriter>(root, true);	// Re-open root node to append items
		wr->WriteInt(__X("c"), 3);
		wr->EndDictionary();	// Must explicitly close the root collection re-opened
		UNITTEST_ASSERT(JE(root).GetString() == __X("{\"a\":1,\"b\":2,\"c\":3}"));

		// Method 3, using JE re-open function
		expr = 
			JE::Reopen(root) + 
				JE::Pair(__X("d"), 4) +
			JE::EDict();		// Must explicitly close the root collection re-opened
		UNITTEST_ASSERT(expr.GetString() == __X("{\"a\":1,\"b\":2,\"c\":3,\"d\":4}"));
	}

	void TestJsonWriteReopenList()
	{
		// Original JE
		JE expr = JE::New() + 
			JE::List() + 
				1 +
			JE::EList();
		ref<JsonNode> root = expr.Complete();	// the root list node to be re-opened

		// Method 1, manually add a new JsonNode
		ref<JsonNode> node = JsonNode::NewIntNode(2);
		root->AddListItem(node);
		UNITTEST_ASSERT(JE(root).GetString() == __X("[1,2]"));

		// Method 2, using JsonWriter re-open function
		ref<JsonWriter> wr = gc_new<JsonWriter>(root, true);	// Re-open root node to append items
		wr->WriteInt(NULL, 3);
		wr->EndList();	// Must explicitly close the root collection re-opened
		UNITTEST_ASSERT(JE(root).GetString() == __X("[1,2,3]"));

		// Method 3, using JE re-open function
		expr = 
			JE::Reopen(root) + 
				4 +
			JE::EList();		// Must explicitly close the root collection re-opened
		UNITTEST_ASSERT(expr.GetString() == __X("[1,2,3,4]"));
	}

	void TestJsonWriteReopenComplex()
	{
		JE eMain = JE::New() +
			JE::Dict() +
				JE::List(__X("keys")) +
					1 +
				JE::EList() +
				JE::Dict(__X("units")) +
					JE::Pair(__X("1"), __X("local")) +
				JE::EDict() +
			JE::EDict();		
		ref<JsonNode> root = eMain.Complete();

		// Add an item to List "keys"
		ref<JsonNode> nodeKeys = root->GetChild(__X("keys"));
		JE::Reopen(nodeKeys) + 2 + JE::EList();

		// Add an item to Dictionary "nodes"
		ref<JsonNode> nodeUnits = root->GetChild(__X("units"));
		JE::Reopen(nodeUnits) + JE::Pair(__X("2"), __X("remote")) + JE::EDict();

		stringx t = eMain.GetString();
		UNITTEST_ASSERT(t == __X("{\"keys\":[1,2],\"units\":{\"1\":\"local\",\"2\":\"remote\"}}"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestJsonWriter);

// -------------------------------------------------------------------------- //
#endif
