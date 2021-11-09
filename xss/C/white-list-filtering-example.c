#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 게시판에서 허용하고자 하는 HTML 태그들을 화이트리스트로 정의하여 필터링한다.
static unsigned char *allowed_formatters[] = 
{
  "b", "big", "blink", "i", "s", "small", "strike", "sub", "sup", "tt", "u",
  "abbr", "acronym", "cite", "code", "del", "dfn", "em", "ins", "kbd", "samp",
  "strong", "var", "dir", "li", "dl", "dd", "dt", "menu", "ol", "ul", "hr",
  "br", "p", "h1", "h2", "h3", "h4", "h5", "h6", "center", "bdo", "blockquote",
   "nobr", "plaintext", "pre", "q", "spacer",
   "a"
 };

#define SKIP_WHITESPACE(p) while (isspace(*p)) p++

static int spc_is_valid_link(const char *input) {
  
  // 요청이 들어온 사이트 링크에 대한 유효성 검증
   static const char *href="href";
   static const char *http = "http://";
   int quoted_string = 0, seen_whitespace = 0;

   if (!isspace(*input)) return 0;
   SKIP_WHITESPACE(input);
   if (strncasecmp(href, input, strlen(href))) return 0;
   input += strlen(href);
   SKIP_WHITESPACE(input);
   if (*input++ != '=') return 0;
   SKIP_WHITESPACE(input);
   if (*input == '"') {
   quoted_string = 1;
   input++;
   }
   if (strncasecmp(http, input, strlen(http))) return 0;
   for (input += strlen(http); *input && *input != '>'; input++) {
     switch (*input) {
     
       case '.': case '/': case '-': case '_':
         break;
     
       case '"':
         if (!quoted_string) return 0;
         SKIP_WHITESPACE(input);
         if (*input != '>') return 0;
         return 1;

        default:

         if (isspace(*input)) {
         if (seen_whitespace && !quoted_string) return 0;
         SKIP_WHITESPACE(input);
         seen_whitespace = 1;
         break;
         }
         if (!isalnum(*input)) return 0;
         break;
       }
   }
   return (*input && !quoted_string);
}

// HTML 연결 문자열에 대한 구성 확인하는 함수
static int spc_allow_tag(const char *input) {
       int i;
       char *tmp;

       if (*input == 'a')
       return spc_is_valid_link(input + 1);
       if (*input == '/') {
       input++;
       SKIP_WHITESPACE(input);
       }
       for (i = 0; i < sizeof(allowed_formatters); i++) {
       if (strncasecmp(allowed_formatters[i], input, strlen(allowed_formatters[i])))
       continue;
       else {
       tmp = input + strlen(allowed_formatters[i]);
       SKIP_WHITESPACE(tmp);
       if (*input == '>') return 1;
     }
  }
  return 0;
}
