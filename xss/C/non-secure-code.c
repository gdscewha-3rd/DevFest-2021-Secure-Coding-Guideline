int XSS(int argc, char* argv[]) {
  unsigned int i = 0;
  char data[1024];
  
// cgiFromString으로 받아온 사용자 입력값이 검증 없이 화면에 출력됩니다.
  giFromString(“user input”, data, sizeof(data));

  printf(cgiOut, “Print user input = %s<br/>”, data);
  fprintf(cgiOut, “</body></html>￦n”);
  
  return 0;
}
