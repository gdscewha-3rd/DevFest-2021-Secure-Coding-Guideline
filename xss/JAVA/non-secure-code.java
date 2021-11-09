<% String keyword = request.getParameter("keyword"); %>
//외부 입력값에 대하여 검증 없이 화면에 출력될 경우 공격스크립트가 포함된 URL을 생성 할 수 있어 안전하지 않다.(Reflected XSS)
// 검색어 : <%=keyword%>
  
//게시판 등의 입력form을 통해 외부값이 DB에 저장되고, 이를 검증 없이 화면에 출력될 경우 공격스크립트가 실행되어 안전하지 않다.(Stored XSS)
//검색결과 : ${m.content}
  
//외부 입력값에 대하여 검증 없이 브라우저에서 실행되는 경우 서버를 거치지 않는 공격스크립트가 포함된 URL을 생성 할 수 있어 안전하지 않다. (DOM 기반 XSS)
<script type="text/javascript">
  document.write("keyword:" + <%=keyword%>);
</script>
