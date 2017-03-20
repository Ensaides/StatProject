#include "Connection.h"
#include "HTMLParser.h"
#include <libxml/HTMLparser.h>

using namespace Parsers::HTML;

// Foward declaration for handlers for elements starting/ending
static void StartElement(void* Userdata, const xmlChar *name, const xmlChar **attributes);
static void EndElement(void* Userdata, const xmlChar *name);
static void CharacterWriter(void* Userdata, const xmlChar* ch, int len);

htmlSAXHandler saxHandler =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	StartElement,
	EndElement,
	NULL,
	CharacterWriter
};

namespace Parsers
{
	HTMLPage HTML::ParseHTML(const std::string &html)
	{
		//  Create a new page to return
		auto ReturnHTMLPage = HTMLPage();
		htmlParserCtxtPtr ctxt;

		ctxt = htmlCreatePushParserCtxt(&saxHandler, &ReturnHTMLPage, "", 0, "",
			XML_CHAR_ENCODING_NONE);

		htmlParseChunk(ctxt, html.c_str(), html.size(), 0);
		htmlParseChunk(ctxt, "", 0, 1);

		htmlFreeParserCtxt(ctxt);

		return ReturnHTMLPage;
	}
}

// The parent index, -1 is an invalid index
int32_t ParentElemIndex = -1;

//  Most of this code is copied shamelessly from a tutorial
void StartElement(void* Userdata, const xmlChar* name, const xmlChar** attributes)
{
	auto ReturnHTMLPage = (HTMLPage*)Userdata;

	//  Create a new elem to add to the return HTML page
	auto NewElement = HTMLElem();
	NewElement.Name = std::string((char*)name);

	// Set the parent elem
	NewElement.ParentElemIndex = ParentElemIndex;

	if (attributes)
	{
		int i = 0;
		while (attributes[i])
		{
			NewElement.Attributes.push_back(std::string((char*)attributes[i]));

			i++;
		}
	}

	// Set the parent element to the last element in the array
	ReturnHTMLPage->Elements.push_back(NewElement);
	ParentElemIndex = ReturnHTMLPage->Elements.size() - 1;

	(void)attributes;
}

void EndElement(void* Userdata, const xmlChar* name)
{
	auto ReturnHTMLPage = (HTMLPage*)Userdata;

	// If this element has ended, set the next parent to the one up the tree
	if (ParentElemIndex > -1)
	{
		ParentElemIndex = ReturnHTMLPage->Elements[ParentElemIndex].ParentElemIndex;
	}
}

void CharacterWriter(void* Userdata, const xmlChar* ch, int len)
{
	auto ReturnHTMLPage = (HTMLPage*)Userdata;

	if (ch)
	{
		ReturnHTMLPage->Elements.back().Text = std::string((char*)ch);
	}
}