
#include "Html.hpp"

Html::Html(ESP8266WebServer * server) {
  m_server = server;
}

void Html::send(String& html) {
  m_server->send(200,"text/html",html);
}
