<% String keyword = request.getParameter("keyword"); %>

  // 방법1. 입력값에 대하여 스크립트 공격가능성이 있는 문자열을 치환한다.
keyword = keyword.replaceAll("&", "&amp;");
keyword = keyword.replaceAll("<", "&lt;");
keyword = keyword.replaceAll(">", "&gt;");
keyword = keyword.replaceAll("￦"", "&quot;");
keyword = keyword.replaceAll("'", "&#x27;");
keyword = keyword.replaceAll("/"", "&#x2F;");
keyword = keyword.replaceAll("(", "&#x28;");
keyword = keyword.replaceAll(")", "&#x29;");
// 검색어 : <%=keyword%>
                             
  //방법2. JSP에서 출력값에 JSTL c:out 을 사용하여 처리한다.
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
// 검색결과 : <c:out value="${m.content}"/>
                             
  //방법3. 잘 만들어진 외부 라이브러리를 활용(NAVER Lucy-XSS-Filter, OWASP ESAPI, OWASP Java-Encoder-Project)
<script type="text/javascript">
  document.write("keyword:" +
  <%=Encoder.encodeForJS(Encoder.encodeForHTML(keyword))%>);
</script>
