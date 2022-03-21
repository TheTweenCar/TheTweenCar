#include <ESP8266WebServer.h>

/*
 * class Html
 * Send Html via a ESP8266WebServer
 */
class Html {
private:
  ESP8266WebServer * m_server;
  
public:
  Html(ESP8266WebServer * server);
  void send(String& html) ;
};


class HtmlPage: public String {
  public:
  HtmlPage(String headers, String body):String(
    String("<html>\n<headers>\n"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n") +
    headers + 
    "\n</headers>\n<body style=\"font-family: monospace;font-size: 200%;\">\n" +
    body +
    "\n</body>\n</html>\n"){};
};

class HtmlTitle: public String {
  public:
  HtmlTitle(String a):String("\n<title>" + a + "</title>\n"){};
};

class HtmlHeader: public String {
  public:
  HtmlHeader(String header,String indent):String("\n<h" + indent + ">" + header + "</h" + indent + ">\n"){};
};

class HtmlLink: public String {
  public:
  HtmlLink(String link,String url):String("\n<a href=\"" + url + "\">" + link + "</a>"){};
};

class HtmlBr: public String {
  public:
  HtmlBr():String("<br>"){ };

};
