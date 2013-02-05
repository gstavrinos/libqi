#! /usr/bin/python2

import tornado
import tornadio2
from qimessaging.session import Session
import sys

url = None

class QiMessagingHandler(tornadio2.conn.SocketConnection):

    def on_open(self, info):
        self.s = Session()
        self.s.connect(url)
        pass

    @tornadio2.event
    def call(self, idm, params):
        service = params["service"]
        method = params["method"]
        if method == "services":
            data = self.s.services()
        else:
            args = params["args"]
            if method == "service":
                o = self.s.service(str(args[0]))
                r = []
                for m in dir(o):
                  try:
                    sigarg = getattr(o, m).__signatures__
                    sigres = getattr(o, m).__sigreturns__
                    docs = getattr(o, m).__docs__
                    s = []
                    for i in range(len(sigarg)):
                      s.append([ sigres[i], sigarg[i], docs[sigarg[i]] ])
                    r.append( {"name": m, "signatures": s } )
                    data = { "name": args[0], "doc": o.__doc__, "functions": r }
                  except:
                    pass
            else:
                o = self.s.service(str(service))
                m = getattr(o, method)
                if args is None:
                  data = m()
                else:
                  data = m(*args)

        self.emit('reply', { "idm": idm, "result": data })

    def on_close(self):
        pass

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: %s SD_URL" % sys.argv[0])
        sys.exit(1)

    url = sys.argv[1]

    # Create tornadio server
    ChatRouter = tornadio2.router.TornadioRouter(QiMessagingHandler)

    # Create socket application
    sock_app = tornado.web.Application(
      ChatRouter.urls,
      socket_io_port = 8002
    )

    # Create HTTP application
    http_app = tornado.web.Application(
      [(r'/(socket.io.js)', tornado.web.StaticFileHandler, {'path': "./"}),
       (r'/(qimessaging.js)', tornado.web.StaticFileHandler, {'path': "./"})]
    )

    # Create http server on port 8001
    http_server = tornado.httpserver.HTTPServer(http_app)
    http_server.listen(8001)

    # Create tornadio server on port 8002, but don't start it yet
    tornadio2.server.SocketServer(sock_app, auto_start=False)

    # Start both servers
    tornado.ioloop.IOLoop.instance().start()
