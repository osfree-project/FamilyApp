/*
Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
See the file COPYING for copying permission.
*/

#ifndef XmlParse_INCLUDED
#define XmlParse_INCLUDED 1

#include <stdlib.h>

#include "expat/expat_setup.h"
                        // V0.9.9 (2001-02-10) [umoeller]
                        // to save the app from having to include this as well

/* #ifndef XMLPARSEAPI
    #ifdef EXPATENTRY
        // #define XMLPARSEAPI(type) type EXPATENTRY
        #define XMLPARSEAPI(type)
            // V0.9.14 (2001-08-09) [umoeller]
    #else
        #define XMLPARSEAPI(type) type
    #endif
#endif */

#ifndef XMLPARSEAPI
#  ifdef __declspec
#    define XMLPARSEAPI __declspec(dllimport)
#  else
#    define XMLPARSEAPI /* nothing */
#  endif
#endif  /* not defined XMLPARSEAPI */

#ifdef __cplusplus
extern "C" {
#endif

typedef void *XML_Parser;

/* Information is UTF-8 encoded. */
typedef char XML_Char;
typedef char XML_LChar;

enum XML_Content_Type
{
    XML_CTYPE_EMPTY = 1,
    XML_CTYPE_ANY,
    XML_CTYPE_MIXED,
    XML_CTYPE_NAME,
    XML_CTYPE_CHOICE,
    XML_CTYPE_SEQ
};

enum XML_Content_Quant
{
    XML_CQUANT_NONE,
    XML_CQUANT_OPT,
    XML_CQUANT_REP,
    XML_CQUANT_PLUS
};

/*
 *@@ XMLCONTENT:
 *      structure passed with the @expat "element declaration handler"
 *      (see XML_SetElementDeclHandler).
 *
 *      The model argument is the root of a tree of XMLCONTENT
 *      nodes. If type equals XML_CTYPE_EMPTY or XML_CTYPE_ANY,
 *      then quant will be XML_CQUANT_NONE, and the other fields
 *      will be zero or NULL.
 *
 *      If type is XML_CTYPE_MIXED:
 *
 *      --  "quant" will be XML_CQUANT_NONE or XML_CQUANT_REP.
 *
 *      --  "numchildren" will contain the number of elements that
 *          are allowed to be mixed in.
 *
 *      --  "children" points to a sub-array of XMLCONTENT structures
 *          that will all have type XML_CTYPE_NAME with no quantification.
 *
 *      Only the root node can be type XML_CTYPE_EMPTY, XML_CTYPE_ANY,
 *      or XML_CTYPE_MIXED.
 *
 *      For type XML_CTYPE_NAME, the name field points to the
 *      name and the numchildren and children fields will be
 *      zero and NULL. The quant field will indicate any
 *      quantifiers placed on the name.
 *
 *      Types XML_CTYPE_CHOICE and XML_CTYPE_SEQ indicate a
 *      choice or sequence respectively. The numchildren field
 *      indicates how many nodes in the choice or sequence and
 *      children points to the nodes.
 *
 *@@added V0.9.9 (2001-02-14) [umoeller]
 */

typedef struct _XMLCONTENT
{
    enum XML_Content_Type       type;
                    // one of:
                    // -- XML_CTYPE_EMPTY   --> quant will be XML_CQUANT_NONE, rest is NULL
                    // -- XML_CTYPE_ANY,    --> quant will be XML_CQUANT_NONE, rest is NULL
                    // -- XML_CTYPE_MIXED,  --> (#PCDATA) gives us this
                    // -- XML_CTYPE_CHOICE, --> choice ("|" list)
                    // -- XML_CTYPE_SEQ     --> sequence (comma list)

                    // -- XML_CTYPE_NAME: used for sub-content if content has CML_CTYPE_MIXED;
                    //                    only "name" is valid for sub-content

    enum XML_Content_Quant      quant;
                    // one of:
                    // -- XML_CQUANT_NONE   --> all fields below are NULL
                    // -- XML_CQUANT_OPT,   question mark
                    // -- XML_CQUANT_REP,   asterisk
                    // -- XML_CQUANT_PLUS   plus sign

    XML_Char                    *name;

    unsigned int                numchildren;

    struct _XMLCONTENT          *children;
} XMLCONTENT, *PXMLCONTENT;


/* This is called for an element declaration. See above for
   description of the model argument. It's the caller's responsibility
   to free model when finished with it.
*/

typedef void (* EXPATENTRY XML_ElementDeclHandler) (void *userData,
                                        const XML_Char *name,
                                        XMLCONTENT *model);

void EXPATENTRY XML_SetElementDeclHandler(XML_Parser parser,
                                            XML_ElementDeclHandler eldecl);

/*
  The Attlist declaration handler is called for *each* attribute. So
  a single Attlist declaration with multiple attributes declared will
  generate multiple calls to this handler. The "default" parameter
  may be NULL in the case of the "#IMPLIED" or "#REQUIRED" keyword.
  The "isrequired" parameter will be true and the default value will
  be NULL in the case of "#REQUIRED". If "isrequired" is true and
  default is non-NULL, then this is a "#FIXED" default.
 */

typedef void (* EXPATENTRY XML_AttlistDeclHandler) (void           *userData,
                                        const XML_Char *elname,
                                        const XML_Char *attname,
                                        const XML_Char *att_type,
                                        const XML_Char *dflt,
                                        int     isrequired);

void EXPATENTRY
XML_SetAttlistDeclHandler(XML_Parser parser,
              XML_AttlistDeclHandler attdecl);


  /* The XML declaration handler is called for *both* XML declarations and
     text declarations. The way to distinguish is that the version parameter
     will be null for text declarations. The encoding parameter may be null
     for XML declarations. The standalone parameter will be -1, 0, or 1
     indicating respectively that there was no standalone parameter in
     the declaration, that it was given as no, or that it was given as yes.
  */

typedef void (* EXPATENTRY XML_XmlDeclHandler) (void        *userData,
                                    const XML_Char  *version,
                                    const XML_Char  *encoding,
                                    int          standalone);

void EXPATENTRY
XML_SetXmlDeclHandler(XML_Parser parser,
              XML_XmlDeclHandler xmldecl);


typedef struct {
  void *(*malloc_fcn)(size_t size);
  void *(*realloc_fcn)(void *ptr, size_t size);
  void (*free_fcn)(void *ptr);
} XML_Memory_Handling_Suite;

/* Constructs a new parser; encoding is the encoding specified by the
external protocol or null if there is none specified. */

XML_Parser EXPATENTRY
XML_ParserCreate(const XML_Char *encoding);

/* Constructs a new parser and namespace processor.  Element type
names and attribute names that belong to a namespace will be expanded;
unprefixed attribute names are never expanded; unprefixed element type
names are expanded only if there is a default namespace. The expanded
name is the concatenation of the namespace URI, the namespace
separator character, and the local part of the name.  If the namespace
separator is '\0' then the namespace URI and the local part will be
concatenated without any separator.  When a namespace is not declared,
the name and prefix will be passed through without expansion. */

XML_Parser EXPATENTRY
XML_ParserCreateNS(const XML_Char *encoding, XML_Char namespaceSeparator);


/* Constructs a new parser using the memory management suit referred to
   by memsuite. If memsuite is NULL, then use the standard library memory
   suite. If namespaceSeparator is non-NULL it creates a parser with
   namespace processing as described above. The character pointed at
   will serve as the namespace separator.

   All further memory operations used for the created parser will come from
   the given suite.
*/

XML_Parser EXPATENTRY
XML_ParserCreate_MM(const XML_Char *encoding,
            const XML_Memory_Handling_Suite *memsuite,
            const XML_Char *namespaceSeparator);

/* atts is array of name/value pairs, terminated by 0;
   names and values are 0 terminated. */

typedef void (* EXPATENTRY XML_StartElementHandler)(void *userData,
                                        const XML_Char *name,
                                        const XML_Char **atts);

typedef void (* EXPATENTRY XML_EndElementHandler)(void *userData,
                                      const XML_Char *name);


/* s is not 0 terminated. */
typedef void (* EXPATENTRY XML_CharacterDataHandler)(void *userData,
                                         const XML_Char *s,
                                         int len);

/* target and data are 0 terminated */
typedef void (* EXPATENTRY XML_ProcessingInstructionHandler)(void *userData,
                                                 const XML_Char *target,
                                                 const XML_Char *data);

/* data is 0 terminated */
typedef void (* EXPATENTRY XML_CommentHandler)(void *userData, const XML_Char *data);

typedef void (* EXPATENTRY XML_StartCdataSectionHandler)(void *userData);
typedef void (* EXPATENTRY XML_EndCdataSectionHandler)(void *userData);

/* This is called for any characters in the XML document for
which there is no applicable handler.  This includes both
characters that are part of markup which is of a kind that is
not reported (comments, markup declarations), or characters
that are part of a construct which could be reported but
for which no handler has been supplied. The characters are passed
exactly as they were in the XML document except that
they will be encoded in UTF-8.  Line boundaries are not normalized.
Note that a byte order mark character is not passed to the default handler.
There are no guarantees about how characters are divided between calls
to the default handler: for example, a comment might be split between
multiple calls. */

typedef void (* EXPATENTRY XML_DefaultHandler)(void *userData,
                                   const XML_Char *s,
                                   int len);

/* This is called for the start of the DOCTYPE declaration, before
   any DTD or internal subset is parsed. */

typedef void (* EXPATENTRY XML_StartDoctypeDeclHandler)(void *userData,
                                            const XML_Char *doctypeName,
                                            const XML_Char *sysid,
                                            const XML_Char *pubid,
                                            int has_internal_subset);

/* This is called for the start of the DOCTYPE declaration when the
closing > is encountered, but after processing any external subset. */
typedef void (* EXPATENTRY XML_EndDoctypeDeclHandler)(void *userData);

/* This is called for entity declarations. The is_parameter_entity
   argument will be non-zero if the entity is a parameter entity, zero
   otherwise.

   For internal entities (<!ENTITY foo "bar">), value will
   be non-null and systemId, publicID, and notationName will be null.
   The value string is NOT null terminated; the length is provided in
   the value_length argument. Since it is legal to have zero-length
   values, do not use this argument to test for internal entities.

   For external entities, value will be null and systemId will be non-null.
   The publicId argument will be null unless a public identifier was
   provided. The notationName argument will have a non-null value only
   for unparsed entity declarations.
*/

typedef void (* EXPATENTRY XML_EntityDeclHandler) (void *userData,
                                       const XML_Char *entityName,
                                       int is_parameter_entity,
                                       const XML_Char *value,
                                       int value_length,
                                       const XML_Char *base,
                                       const XML_Char *systemId,
                                       const XML_Char *publicId,
                                       const XML_Char *notationName);

void EXPATENTRY
XML_SetEntityDeclHandler(XML_Parser parser,
             XML_EntityDeclHandler handler);

/* OBSOLETE -- OBSOLETE -- OBSOLETE
   This handler has been superceded by the EntityDeclHandler above.
   It is provided here for backward compatibility.
This is called for a declaration of an unparsed (NDATA)
entity.  The base argument is whatever was set by XML_SetBase.
The entityName, systemId and notationName arguments will never be null.
The other arguments may be. */

typedef void (* EXPATENTRY XML_UnparsedEntityDeclHandler)(void *userData,
                                              const XML_Char *entityName,
                                              const XML_Char *base,
                                              const XML_Char *systemId,
                                              const XML_Char *publicId,
                                              const XML_Char *notationName);

/* This is called for a declaration of notation.
The base argument is whatever was set by XML_SetBase.
The notationName will never be null.  The other arguments can be. */

typedef void (* EXPATENTRY XML_NotationDeclHandler)(void *userData,
                                        const XML_Char *notationName,
                                        const XML_Char *base,
                                        const XML_Char *systemId,
                                        const XML_Char *publicId);

/* When namespace processing is enabled, these are called once for
each namespace declaration. The call to the start and end element
handlers occur between the calls to the start and end namespace
declaration handlers. For an xmlns attribute, prefix will be null.
For an xmlns="" attribute, uri will be null. */

typedef void (* EXPATENTRY XML_StartNamespaceDeclHandler)(void *userData,
                                              const XML_Char *prefix,
                                              const XML_Char *uri);

typedef void (* EXPATENTRY XML_EndNamespaceDeclHandler)(void *userData,
                                            const XML_Char *prefix);

/* This is called if the document is not standalone (it has an
external subset or a reference to a parameter entity, but does not
have standalone="yes"). If this handler returns 0, then processing
will not continue, and the parser will return a
ERROR_EXPAT_NOT_STANDALONE error. */

typedef int (* EXPATENTRY XML_NotStandaloneHandler)(void *userData);

/* This is called for a reference to an external parsed general entity.
The referenced entity is not automatically parsed.
The application can parse it immediately or later using
XML_ExternalEntityParserCreate.
The parser argument is the parser parsing the entity containing the reference;
it can be passed as the parser argument to XML_ExternalEntityParserCreate.
The systemId argument is the system identifier as specified in the entity
declaration; it will not be null.
The base argument is the system identifier that should be used as the base for
resolving systemId if systemId was relative; this is set by XML_SetBase;
it may be null.
The publicId argument is the public identifier as specified in the entity
declaration, or null if none was specified; the whitespace in the public
identifier will have been normalized as required by the XML spec.
The context argument specifies the parsing context in the format
expected by the context argument to
XML_ExternalEntityParserCreate; context is valid only until the handler
returns, so if the referenced entity is to be parsed later, it must be copied.
The handler should return 0 if processing should not continue because of
a fatal error in the handling of the external entity.
In this case the calling parser will return an
ERROR_EXPAT_EXTERNAL_ENTITY_HANDLING error.
Note that unlike other handlers the first argument is the parser, not
userData. */

typedef int (* EXPATENTRY XML_ExternalEntityRefHandler)(void *userData,     // added V0.9.14 (2001-08-09) [umoeller]
                                            XML_Parser parser,
                                            const XML_Char *context,
                                            const XML_Char *base,
                                            const XML_Char *systemId,
                                            const XML_Char *publicId);

/* This structure is filled in by the XML_UnknownEncodingHandler
to provide information to the parser about encodings that are unknown
to the parser.
The map[b] member gives information about byte sequences
whose first byte is b.
If map[b] is c where c is >= 0, then b by itself encodes the Unicode scalar
value c.
If map[b] is -1, then the byte sequence is malformed.
If map[b] is -n, where n >= 2, then b is the first byte of an n-byte
sequence that encodes a single Unicode scalar value.
The data member will be passed as the first argument to the convert function.
The convert function is used to convert multibyte sequences;
s will point to a n-byte sequence where map[(unsigned char)*s] == -n.
The convert function must return the Unicode scalar value
represented by this byte sequence or -1 if the byte sequence is malformed.
The convert function may be null if the encoding is a single-byte encoding,
that is if map[b] >= -1 for all bytes b.
When the parser is finished with the encoding, then if release is not null,
it will call release passing it the data member;
once release has been called, the convert function will not be called again.

Expat places certain restrictions on the encodings that are supported
using this mechanism.

1. Every ASCII character that can appear in a well-formed XML document,
other than the characters

  $@\^`{}~

must be represented by a single byte, and that byte must be the
same byte that represents that character in ASCII.

2. No character may require more than 4 bytes to encode.

3. All characters encoded must have Unicode scalar values <= 0xFFFF, (i.e.,
characters that would be encoded by surrogates in UTF-16 are  not
allowed).  Note that this restriction doesn't apply to the built-in
support for UTF-8 and UTF-16.

4. No Unicode character may be encoded by more than one distinct sequence
of bytes. */

typedef struct {
  int map[256];
  void *data;
  int (* EXPATENTRY convert)(void *data, const char *s);
  void (* EXPATENTRY release)(void *data);
} XML_Encoding;

/* This is called for an encoding that is unknown to the parser.
The encodingHandlerData argument is that which was passed as the
second argument to XML_SetUnknownEncodingHandler.
The name argument gives the name of the encoding as specified in
the encoding declaration.
If the callback can provide information about the encoding,
it must fill in the XML_Encoding structure, and return 1.
Otherwise it must return 0.
If info does not describe a suitable encoding,
then the parser will return an XML_UNKNOWN_ENCODING error. */

typedef int (* EXPATENTRY XML_UnknownEncodingHandler)(void *encodingHandlerData,
                                          const XML_Char *name,
                                          XML_Encoding *info);

void EXPATENTRY
XML_SetElementHandler(XML_Parser parser,
              XML_StartElementHandler start,
              XML_EndElementHandler end);

void EXPATENTRY
XML_SetStartElementHandler(XML_Parser, XML_StartElementHandler);

void EXPATENTRY
XML_SetEndElementHandler(XML_Parser, XML_EndElementHandler);

void EXPATENTRY
XML_SetCharacterDataHandler(XML_Parser parser,
                XML_CharacterDataHandler handler);

void EXPATENTRY
XML_SetProcessingInstructionHandler(XML_Parser parser,
                    XML_ProcessingInstructionHandler handler);
void EXPATENTRY
XML_SetCommentHandler(XML_Parser parser,
                      XML_CommentHandler handler);

void EXPATENTRY
XML_SetCdataSectionHandler(XML_Parser parser,
               XML_StartCdataSectionHandler start,
               XML_EndCdataSectionHandler end);

void EXPATENTRY
XML_SetStartCdataSectionHandler(XML_Parser parser,
                                XML_StartCdataSectionHandler start);

void EXPATENTRY
XML_SetEndCdataSectionHandler(XML_Parser parser,
                              XML_EndCdataSectionHandler end);

/* This sets the default handler and also inhibits expansion of
internal entities.  The entity reference will be passed to the default
handler. */

void EXPATENTRY
XML_SetDefaultHandler(XML_Parser parser,
              XML_DefaultHandler handler);

/* This sets the default handler but does not inhibit expansion of
internal entities.  The entity reference will not be passed to the
default handler. */

void EXPATENTRY
XML_SetDefaultHandlerExpand(XML_Parser parser,
                    XML_DefaultHandler handler);

void EXPATENTRY
XML_SetDoctypeDeclHandler(XML_Parser parser,
              XML_StartDoctypeDeclHandler start,
              XML_EndDoctypeDeclHandler end);

void EXPATENTRY
XML_SetStartDoctypeDeclHandler(XML_Parser parser,
                   XML_StartDoctypeDeclHandler start);

void EXPATENTRY
XML_SetEndDoctypeDeclHandler(XML_Parser parser,
                 XML_EndDoctypeDeclHandler end);

void EXPATENTRY
XML_SetUnparsedEntityDeclHandler(XML_Parser parser,
                 XML_UnparsedEntityDeclHandler handler);

void EXPATENTRY
XML_SetNotationDeclHandler(XML_Parser parser,
               XML_NotationDeclHandler handler);

void EXPATENTRY
XML_SetNamespaceDeclHandler(XML_Parser parser,
                XML_StartNamespaceDeclHandler start,
                XML_EndNamespaceDeclHandler end);

void EXPATENTRY
XML_SetStartNamespaceDeclHandler(XML_Parser parser,
                 XML_StartNamespaceDeclHandler start);

void EXPATENTRY
XML_SetEndNamespaceDeclHandler(XML_Parser parser,
                   XML_EndNamespaceDeclHandler end);

void EXPATENTRY
XML_SetNotStandaloneHandler(XML_Parser parser,
                XML_NotStandaloneHandler handler);

void EXPATENTRY
XML_SetExternalEntityRefHandler(XML_Parser parser,
                XML_ExternalEntityRefHandler handler);

/* If a non-null value for arg is specified here, then it will be passed
as the first argument to the external entity ref handler instead
of the parser object. */
void EXPATENTRY
XML_SetExternalEntityRefHandlerArg(XML_Parser, void *arg);

void EXPATENTRY
XML_SetUnknownEncodingHandler(XML_Parser parser,
                  XML_UnknownEncodingHandler handler,
                  void *encodingHandlerData);

/* This can be called within a handler for a start element, end element,
processing instruction or character data.  It causes the corresponding
markup to be passed to the default handler. */
void EXPATENTRY
XML_DefaultCurrent(XML_Parser parser);

/* If do_nst is non-zero, and namespace processing is in effect, and
   a name has a prefix (i.e. an explicit namespace qualifier) then
   that name is returned as a triplet in a single
   string separated by the separator character specified when the parser
   was created: URI + sep + local_name + sep + prefix.

   If do_nst is zero, then namespace information is returned in the
   default manner (URI + sep + local_name) whether or not the names
   has a prefix.
*/

void EXPATENTRY
XML_SetReturnNSTriplet(XML_Parser parser, int do_nst);

/* This value is passed as the userData argument to callbacks. */
void EXPATENTRY
XML_SetUserData(XML_Parser parser, void *userData);

/* Returns the last value set by XML_SetUserData or null. */
#define XML_GetUserData(parser) (*(void **)(parser))

/* This is equivalent to supplying an encoding argument
to XML_ParserCreate. It must not be called after XML_Parse
or XML_ParseBuffer. */

int EXPATENTRY
XML_SetEncoding(XML_Parser parser, const XML_Char *encoding);

/* If this function is called, then the parser will be passed
as the first argument to callbacks instead of userData.
The userData will still be accessible using XML_GetUserData. */

void EXPATENTRY
XML_UseParserAsHandlerArg(XML_Parser parser);

/* Sets the base to be used for resolving relative URIs in system
identifiers in declarations.  Resolving relative identifiers is left
to the application: this value will be passed through as the base
argument to the XML_ExternalEntityRefHandler, XML_NotationDeclHandler
and XML_UnparsedEntityDeclHandler. The base argument will be copied.
Returns zero if out of memory, non-zero otherwise. */

int EXPATENTRY
XML_SetBase(XML_Parser parser, const XML_Char *base);

const XML_Char * EXPATENTRY
XML_GetBase(XML_Parser parser);

/* Returns the number of the attribute/value pairs passed in last call
to the XML_StartElementHandler that were specified in the start-tag
rather than defaulted. Each attribute/value pair counts as 2; thus
this correspondds to an index into the atts array passed to the
XML_StartElementHandler. */

int EXPATENTRY
XML_GetSpecifiedAttributeCount(XML_Parser parser);

/* Returns the index of the ID attribute passed in the last call to
XML_StartElementHandler, or -1 if there is no ID attribute.  Each
attribute/value pair counts as 2; thus this correspondds to an index
into the atts array passed to the XML_StartElementHandler. */

int EXPATENTRY
XML_GetIdAttributeIndex(XML_Parser parser);

/* Parses some input. Returns 0 if a fatal error is detected.
The last call to XML_Parse must have isFinal true;
len may be zero for this call (or any other). */
int EXPATENTRY
XML_Parse(XML_Parser parser, const char *s, int len, int isFinal);

void * EXPATENTRY
XML_GetBuffer(XML_Parser parser, int len);

int EXPATENTRY
XML_ParseBuffer(XML_Parser parser, int len, int isFinal);

/* Creates an XML_Parser object that can parse an external general
entity; context is a '\0'-terminated string specifying the parse
context; encoding is a '\0'-terminated string giving the name of the
externally specified encoding, or null if there is no externally
specified encoding.  The context string consists of a sequence of
tokens separated by formfeeds (\f); a token consisting of a name
specifies that the general entity of the name is open; a token of the
form prefix=uri specifies the namespace for a particular prefix; a
token of the form =uri specifies the default namespace.  This can be
called at any point after the first call to an
ExternalEntityRefHandler so longer as the parser has not yet been
freed.  The new parser is completely independent and may safely be
used in a separate thread.  The handlers and userData are initialized
from the parser argument.  Returns 0 if out of memory.  Otherwise
returns a new XML_Parser object. */
XML_Parser EXPATENTRY
XML_ExternalEntityParserCreate(XML_Parser parser,
                   const XML_Char *context,
                   const XML_Char *encoding);

enum XML_ParamEntityParsing {
  XML_PARAM_ENTITY_PARSING_NEVER,
  XML_PARAM_ENTITY_PARSING_UNLESS_STANDALONE,
  XML_PARAM_ENTITY_PARSING_ALWAYS
};

/* Controls parsing of parameter entities (including the external DTD
subset). If parsing of parameter entities is enabled, then references
to external parameter entities (including the external DTD subset)
will be passed to the handler set with
XML_SetExternalEntityRefHandler.  The context passed will be 0.
Unlike external general entities, external parameter entities can only
be parsed synchronously.  If the external parameter entity is to be
parsed, it must be parsed during the call to the external entity ref
handler: the complete sequence of XML_ExternalEntityParserCreate,
XML_Parse/XML_ParseBuffer and XML_ParserFree calls must be made during
this call.  After XML_ExternalEntityParserCreate has been called to
create the parser for the external parameter entity (context must be 0
for this call), it is illegal to make any calls on the old parser
until XML_ParserFree has been called on the newly created parser.  If
the library has been compiled without support for parameter entity
parsing (ie without XML_DTD being defined), then
XML_SetParamEntityParsing will return 0 if parsing of parameter
entities is requested; otherwise it will return non-zero. */

int EXPATENTRY
XML_SetParamEntityParsing(XML_Parser parser,
              enum XML_ParamEntityParsing parsing);

/* XMLERROR {
    ERROR_EXPAT_NONE,
    ERROR_EXPAT_NO_MEMORY,
    ERROR_EXPAT_SYNTAX,
    ERROR_EXPAT_NO_ELEMENTS,
    ERROR_EXPAT_INVALID_TOKEN,
    ERROR_EXPAT_UNCLOSED_TOKEN,
    ERROR_EXPAT_PARTIAL_CHAR,
    ERROR_EXPAT_TAG_MISMATCH,
    ERROR_EXPAT_DUPLICATE_ATTRIBUTE,
    ERROR_EXPAT_JUNK_AFTER_DOC_ELEMENT,
    ERROR_EXPAT_PARAM_ENTITY_REF,
    ERROR_EXPAT_UNDEFINED_ENTITY,
    ERROR_EXPAT_RECURSIVE_ENTITY_REF,
    ERROR_EXPAT_ASYNC_ENTITY,
    ERROR_EXPAT_BAD_CHAR_REF,
    ERROR_EXPAT_BINARY_ENTITY_REF,
    ERROR_EXPAT_ATTRIBUTE_EXTERNAL_ENTITY_REF,
    ERROR_EXPAT_MISPLACED_XML_PI,
    ERROR_EXPAT_UNKNOWN_ENCODING,
    ERROR_EXPAT_INCORRECT_ENCODING,
    ERROR_EXPAT_UNCLOSED_CDATA_SECTION,
    ERROR_EXPAT_EXTERNAL_ENTITY_HANDLING,
    ERROR_EXPAT_NOT_STANDALONE,
    ERROR_EXPAT_UNEXPECTED_STATE
}; */

#define ERROR_XML_FIRST         40000   // first error code used

typedef unsigned long XMLERROR;     // V0.9.12 (2001-05-20) [umoeller]

#define ERROR_EXPAT_NONE                0
#define ERROR_EXPAT_NO_MEMORY           (ERROR_XML_FIRST)
#define ERROR_EXPAT_SYNTAX              (ERROR_XML_FIRST + 1)
#define ERROR_EXPAT_NO_ELEMENTS         (ERROR_XML_FIRST + 2)
#define ERROR_EXPAT_INVALID_TOKEN       (ERROR_XML_FIRST + 3)
#define ERROR_EXPAT_UNCLOSED_TOKEN      (ERROR_XML_FIRST + 4)
#define ERROR_EXPAT_PARTIAL_CHAR        (ERROR_XML_FIRST + 5)
#define ERROR_EXPAT_TAG_MISMATCH        (ERROR_XML_FIRST + 6)
#define ERROR_EXPAT_DUPLICATE_ATTRIBUTE (ERROR_XML_FIRST + 7)
#define ERROR_EXPAT_JUNK_AFTER_DOC_ELEMENT (ERROR_XML_FIRST + 8)
#define ERROR_EXPAT_PARAM_ENTITY_REF    (ERROR_XML_FIRST + 10)
#define ERROR_EXPAT_UNDEFINED_ENTITY    (ERROR_XML_FIRST + 11)
#define ERROR_EXPAT_RECURSIVE_ENTITY_REF (ERROR_XML_FIRST + 12)
#define ERROR_EXPAT_ASYNC_ENTITY        (ERROR_XML_FIRST + 13)
#define ERROR_EXPAT_BAD_CHAR_REF        (ERROR_XML_FIRST + 14)
#define ERROR_EXPAT_BINARY_ENTITY_REF   (ERROR_XML_FIRST + 15)
#define ERROR_EXPAT_ATTRIBUTE_EXTERNAL_ENTITY_REF (ERROR_XML_FIRST + 16)
#define ERROR_EXPAT_MISPLACED_XML_PI    (ERROR_XML_FIRST + 17)
#define ERROR_EXPAT_UNKNOWN_ENCODING    (ERROR_XML_FIRST + 18)
#define ERROR_EXPAT_INCORRECT_ENCODING  (ERROR_XML_FIRST + 19)
#define ERROR_EXPAT_UNCLOSED_CDATA_SECTION (ERROR_XML_FIRST + 20)
#define ERROR_EXPAT_EXTERNAL_ENTITY_HANDLING (ERROR_XML_FIRST + 21)
#define ERROR_EXPAT_NOT_STANDALONE      (ERROR_XML_FIRST + 22)
#define ERROR_EXPAT_UNEXPECTED_STATE    (ERROR_XML_FIRST + 23)

/* If XML_Parse or XML_ParseBuffer have returned 0, then XML_GetErrorCode
returns information about the error. */

XMLERROR EXPATENTRY
XML_GetErrorCode(XML_Parser parser);

/* These functions return information about the current parse location.
They may be called when XML_Parse or XML_ParseBuffer return 0;
in this case the location is the location of the character at which
the error was detected.
They may also be called from any other callback called to report
some parse event; in this the location is the location of the first
of the sequence of characters that generated the event. */

int EXPATENTRY XML_GetCurrentLineNumber(XML_Parser parser);
int EXPATENTRY XML_GetCurrentColumnNumber(XML_Parser parser);
long EXPATENTRY XML_GetCurrentByteIndex(XML_Parser parser);

/* Return the number of bytes in the current event.
Returns 0 if the event is in an internal entity. */

int EXPATENTRY
XML_GetCurrentByteCount(XML_Parser parser);

/* If XML_CONTEXT_BYTES is defined, returns the input buffer, sets
   the integer pointed to by offset to the offset within this buffer
   of the current parse position, and sets the integer pointed to by size
   to the size of this buffer (the number of input bytes). Otherwise
   returns a null pointer. Also returns a null pointer if a parse isn't
   active.

   NOTE: The character pointer returned should not be used outside
   the handler that makes the call. */

const char * EXPATENTRY
XML_GetInputContext(XML_Parser parser,
            int *offset,
            int *size);

/* For backwards compatibility with previous versions. */
#define XML_GetErrorLineNumber XML_GetCurrentLineNumber
#define XML_GetErrorColumnNumber XML_GetCurrentColumnNumber
#define XML_GetErrorByteIndex XML_GetCurrentByteIndex

/* Frees memory used by the parser. */
void EXPATENTRY
XML_ParserFree(XML_Parser parser);

/* Returns a string describing the error. */
const XML_LChar * EXPATENTRY
XML_ErrorString(int code);

/* Return a string containing the version number of this expat */
const XML_LChar * EXPATENTRY
XML_ExpatVersion(void);

typedef struct {
  int major;
  int minor;
  int micro;
} XML_Expat_Version;

/* Return an XML_Expat_Version structure containing numeric version
   number information for this version of expat */

XML_Expat_Version EXPATENTRY
XML_ExpatVersionInfo(void);

#define XML_MAJOR_VERSION @EXPAT_MAJOR_VERSION@
#define XML_MINOR_VERSION @EXPAT_MINOR_VERSION@
#define XML_MICRO_VERSION @EXPAT_EDIT@

#ifdef __cplusplus
}
#endif

#endif /* not XmlParse_INCLUDED */
