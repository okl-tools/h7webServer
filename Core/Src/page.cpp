#include "page.h"
#include <stdio.h>
#include <stdint.h>


#include "PFunction.h"
#include "PTools.h"
#include "PWriter.h"
#include "ObjectMemPool.h"
#include "PStack.h"
#include "json/Node.h"
#include "somePages.h"
#include "PMutex.h"
#include "PRequest.h"

extern "C" {
#include "main.h" // c methods like HAL_Delay
void http_write (struct netconn * conn, const char * data, size_t len);
}


using namespace ptools;
using namespace ptools::json;


void send_http_404_html_pstring(PWriter & out, const PString & sPATH)
{
    out.sprintLF("HTTP/1.1 404 Not Found");
    out.sprintLF("Content-Type: text/html");
    out.sprintLF("Connection: close");
    out.sprintLF("");

    out.sprintLF("<!DOCTYPE html>");
    out.sprintLF("<html>");
    out.sprintLF("<head><title>404 Not Found</title></head>");
    out.sprintLF("<body>");
    out.sprintLF("<h1>404 - Seite nicht gefunden</h1>");
    out.sprintF ("<p>Die Datei <b>$</b> wurde auf diesem Server nicht gefunden.</p>", sPATH);
    out.sprintLF("<hr/>");
    out.sprintLF("<a href='/'>Zurück zur Startseite</a>");
    out.sprintLF("</body>");
    out.sprintLF("</html>");
}
void send_http_404_html(PWriter & s, const char *pStrFile)
{
    s.sprintLF("HTTP/1.1 404 Not Found");
    s.sprintLF("Content-Type: text/html");
    s.sprintLF("Connection: close");
    s.sprintLF("");

    s.sprintLF("<!DOCTYPE html>");
    s.sprintLF("<html>");
    s.sprintLF("<head>");
    s.sprintLF("<title>404 Not Found</title>");
    s.sprintLF("<meta charset='UTF-8'>");
    s.sprintLF("</head>");
    s.sprintLF("<body>");
    s.sprintLF("<h1>404 - File not found</h1>");
//    s.sprintF ("<p>File <b>$</b> does not exist on server.</p>", pStrFile);
    s.sprintF ("<p>The requested file <b>$</b> is not available on this server.</p>", pStrFile);
    s.sprintLF("<hr/>");
    s.sprintLF("Please check the URL or <a href='/'>return to the homepage.</a>");
    s.sprintLF("</body>");
    s.sprintLF("</html>");
}

void send_http_404(PWriter & out)
{
    out.sprintLF("HTTP/1.1 404 Not Found");
    out.sprintLF("Content-Type: text/plain");
    out.sprintLF("Connection: close");
    out.sprintLF(""); // leere Zeile trennt Header von Body
    out.sprintLF("404 - Not Found");
}

void send_http_error_plain(PWriter & out, int code, const char * msg)
{
    out.sprintF("HTTP/1.1 $ $", code, msg);
    out.sprintLF("Content-Type: text/plain");
    out.sprintLF("Connection: close");
    out.sprintLF("");
    out.sprintF("$ - $", code, msg);
}


void create_page (PString & s)
{
    s.sprintLF("<html>");
    s.sprintLF("<head>");
    s.sprintLF("<meta charset='UTF-8'>");
    s.sprintLF("<title>Hallo</title>");
    s.sprintLF("</head>");
    s.sprintLF("<body>");
    s.sprintLF("<h1>Hallo ${vorName}</h1>");
    s.sprintLF("<p>Du siehst hier eine Web-Interaktion<br>");
    s.sprintLF("über mein Intranet mit dem<br>");
    s.sprintLF("${board}, ${processor}</p>");
    s.sprintLF("<p/>Läuft seid:<b>${time}</b></p>");
    s.sprintLF("<ul>");
    s.sprintLF("<li>C, C++</li>");
    s.sprintLF("<li>RTOS</li>");
    s.sprintLF("<li>LWIP</li>");
    s.sprintLF("</ul>");
    s.sprintLF("<hr/>");
    s.sprintLF("<a href='/'>HOME</a>");
    s.sprintLF("</body>");
    s.sprintLF("</html>");
}

void create_page_x (PString & s)
{
    s.sprintLF("<html>");
    s.sprintLF("<head>");
    s.sprintLF("<meta charset='UTF-8'>");
    s.sprintLF("<title>Hallo</title>");
    s.sprintLF("</head>");
    s.sprintLF("<body>");
    s.sprintLF("<h1>Hallo Jodel</h1>");
    s.sprintLF("<p>Du siehst hier eine Web-Interaktion<br>");
    s.sprintLF("<hr/>");
    s.sprintLF("<a href='/'>HOME</a>");
    s.sprintLF("</body>");
    s.sprintLF("</html>");
}

void socket_tx_wrapper (void * theHandle, const uint8_t * data, uint16_t len)
{
    netconn * conn = (netconn *) theHandle;

    http_write(conn, (const char *) data, len);

}




static const char * page_person_danke ()
{
    return "HTTP/1.0 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "\r\n"
           "Person wurde angelegt\r\n"
           "und mehr Blödsinn\r\n"
           "URK\r\n";
}



// if (strncmp((char const *) buf, "GET /STM32H7xx_files/ST.gif", 27) == 0)

// <<<{"led_yellow":553,"led_red":1000}>>>


static void rr6 (const char * pBufRequest, uint16_t reqBuflen, struct netconn * conn)
{
    if (!pBufRequest)
    {
        pp("pBufRequest == nullptr");
        return;
    }
    pp("rr,6 c");

    if (pBufRequest)
    {
        pp_print_buf(pBufRequest, reqBuflen);
        //pp("buf request<<<$>>>", pBufRequest);
    }

    if (strstr(pBufRequest, "POST /"))
    {
        const char * pPageDanke = page_person_danke();
//        pp("------$------", pPageDanke);
        http_write(conn, pPageDanke, string_len(pPageDanke));
    }
    else
    {
        const char * pPage = page_person();
        http_write(conn, pPage, string_len(pPage));
    }

}

//------------------------------------------------------------

ObjectMemPoolWrapper<16, 1500, 100> memObj;

static void write_page_person_danke (const char * pBufRequest, uint16_t reqBuflen, PWriterSocket & pw)
{
    pp("page::write_page_person_danke 3, with PWriterSocket");

//    IMemoryPool & pool = memGlobal.pool;


    pp("###-1");
    pp_print_buf(pBufRequest, reqBuflen);
    pp("###-2");



    memObj.show_info();

    pp("---- lab 1bbb ------");



    const char * pJson = strstr(pBufRequest, "\r\n\r\n");

    if (pJson)
    {
        int pos = pJson - pBufRequest;
        int len = reqBuflen - pos;
        if (len > 4)
        {
            pJson +=4;
            len -=4;
//            Scanner scanner(pBufRequest, reqBuflen, handler);
//            SimpleHandler handler;
            NodeBuilder handler(memObj);
            Scanner scanner(pJson, len, handler);
            scanner.scan_json_data();
//            scanner.debug_run();
            pp("---- lab 2 ------");
            handler.root.show(4);
            pp("---- lab 3 ------");
//            memGlobal.show_info(8);

//            pool.clear();
        }
    }

    pp("---- lab 4 ------");


    pw.sprintCRLF("HTTP/1.0 $ OK", 200);
    pw.sprintCRLF("Content-Type: $", "text/plain");
    pw.sprintCRLF();
    pw.sprintLF("Person wurde angelegt");
    pw.sprintLF("und mehr Quark");
    pw.sprintLF("URK und so");
}

static void write_LED (const char * pBufRequest, uint16_t reqBuflen, PWriterSocket & pw)
{
//    pp("page::write_LED, with PWriterSocket");

//    ARRAY_HANDLES ar(buf_ar, OBJECTS_COUNT);
//    MemoryPool pool(BLOCK_SIZE, BLOCK_COUNT, buf, &ar);

    IObjectMemPool & pool = memObj.get_pool();


    const char * pJson = strstr(pBufRequest, "\r\n\r\n");

    if (pJson)
    {
        int pos = pJson - pBufRequest;
        int len = reqBuflen - pos;
        if (len > 4)
        {
            pJson +=4;
            len -=4;
            NodeBuilder builder(pool);
            Scanner scanner(pJson, len, builder);
            scanner.scan_json_data();


            Node & nRoot = builder.root;

            pp(8, "Node");
            nRoot.show(12);

            Node * n = nRoot.query(".led_yellow");
            if (n)
            {
//                pp("yellow blink:$", n->numberValue);
                set_blinky_yellow_delay(n->numberValue);
            }

            n = nRoot.query(".led_red");
            if (n)
            {
//                pp("red blink:$", n->numberValue);
                set_blinky_red_delay(n->numberValue);
            }
            pp();

            //nRoot.show();


//            pool.clear();
        }
    }


    pw.sprintCRLF("HTTP/1.0 $ OK", 200);
    pw.sprintCRLF("Content-Type: $", "text/plain");
    pw.sprintCRLF("Content-Length: $", 2);
    pw.sprintCRLF();
    pw.sprintLF("OK");
}




static void write_page (PWriterSocket & writerSocket, const char * pPage)
{
    if (pPage == nullptr)
    {
        pp("page::write_page, pPage == nullptr");
        return;
    }
    const uint32_t szPage = string_len(pPage);

    writerSocket.sprintCRLF("HTTP/1.0 $ OK", 200);
    writerSocket.sprintCRLF("Content-Type: $", "text/html");
    writerSocket.sprintCRLF("Content-Length: $", szPage);
    writerSocket.sprintCRLF();

    writerSocket.write_mem(pPage, szPage);
}


static void cpp_request_response (const char * pBufRequest, uint16_t reqBuflen, struct netconn * conn)
{

    PWriterSocket writerSocket(conn, socket_tx_wrapper);

    if (!pBufRequest || reqBuflen < 5)
    {
        send_http_error_plain(writerSocket, 500, "Internal Server Error");
        return;
    }

    IObjectMemPool & pool = memObj.get_pool();


    PStringView viewRequest(pBufRequest, PStringView::BORDER::CRLF);

    PRequest request = PRequest::create_request(viewRequest);
    request.show(4);

    if (request.is_GET())
    {
        pp_first_line(pBufRequest);

//        if (viewRequest.starts_with("GET /") || viewRequest.starts_with("GET /blinky"))
        if (request.requestTarget == "/")
        {
            write_page(writerSocket, page_main());
        }
        else if (request.requestTarget == "/blinky")
        {
//            page_led(writerSocket, conn);
            write_page(writerSocket, page_blink_control());
        }
        else
        {

            PStringView ext = request.requestTarget.get_file_extension();
            if (ext.is_file_extension("html", "htm"))
            {
                PString psFile = request.requestTarget.to_PString(pool);
                send_http_404_html(writerSocket, psFile);
            }
            else
            {
                send_http_404(writerSocket);
            }
        }
    }
//    else if (viewRequest.starts_with("POST"))
    else if (request.is_POST())
    {
        write_LED(pBufRequest, reqBuflen, writerSocket);

    }


}


extern "C" {
void request_response (const char * pBufRequest, uint16_t reqBuflen, void * conn)
{
    cpp_request_response(pBufRequest, reqBuflen, (struct netconn *) conn);
//    rr6(pBufRequest, reqBuflen, (struct netconn *) conn);
    //rr8_LED(pBufRequest, reqBuflen, (struct netconn *) conn);
}
}


