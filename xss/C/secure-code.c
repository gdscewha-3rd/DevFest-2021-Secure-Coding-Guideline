int XSS(int argc, char* argv[]) {
  unsigned int i = 0;
  char data[1024];

  cgiFromString(“user input”, data, sizeof(data));

// data에 위험한 문자열을 검사하는 코드를 추가한다.
  if(strchr(p, ‘<’)) return;
  if(strchr(p, ‘>’)) return;

  fprintf(cgiOut, “Print user input = %s<br/>”, data);
  fprintf(cgiOut, “</body></html>￦n”);

  return 0;
}
