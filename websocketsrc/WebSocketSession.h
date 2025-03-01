#pragma once

#include <string>
#include <map>
#include <memory>

enum MyOpCode : unsigned char
{
    CONTINUATION_FRAME = 0x0,
    TEXT_FRAME         = 0x1,
    BINARY_FRAME       = 0x2,
    RESERVED1          = 0x3,
    RESERVED2          = 0x4,
    RESERVED3          = 0x5,
    RESERVED4          = 0x6,
    RESERVED5          = 0x7,
    CLOSE              = 0x8,
    PING               = 0x9,
    PONG               = 0xA,
    RESERVED6          = 0xB,
    RESERVED7          = 0xC,
    RESERVED8          = 0xD,
    RESERVED9          = 0xE,
    RESERVED10         = 0xF,
};

class WebSocketSession
{
public:
    WebSocketSession();
    virtual ~WebSocketSession() = default;

    WebSocketSession(const WebSocketSession& rhs) = delete;
    WebSocketSession& operator = (const WebSocketSession& rhs) = delete;

public:
    const char* getClientInfo() const
    {
        return m_strClientInfo.c_str();
    }

    const char* getUserAgent() const
    {
        return m_strUserAgent.c_str();
    }


    std::string getHeader(const char* headerField) const;
    std::string getHeaderIgnoreCase(const char* headerField)const;

    std::string getUrl() const
    {
        return m_strURL;
    }

    std::string getParams() const
    {
        return m_strParams;
    }
    bool getUpdateToWebSocket()
    {
        return m_bClientCompressed;
    }

    void close();

    void send(const std::string& data, std::string& login,  int32_t opcode = MyOpCode::TEXT_FRAME, bool compress = false);

    void send(const char* data, size_t dataLength, int32_t opcode = MyOpCode::TEXT_FRAME, bool compress = false);

    void sendAndClose(const char* data, size_t dataLength, bool conpress = false);

    virtual void onConnetct()
    {

    }

    virtual void onDisconnect()
    {

    }

    virtual bool onMessage(const std::string& strClientMsg)
    {
        return false;
    }

    virtual void onPing()
    {
    }

    virtual void onPong()
    {
    }

    void onRead( std::string& buf, std::string& res);

private:
    bool handleHandshake(const std::string& data, std::string& res);
    bool decodePackage(const std::string& str, std::string& res);
    bool parseHttpPath(const std::string& str);
    void makeUpgradeResponse(const char* secWebSocketAccept, std::string& response);
    void unmaskData(std::string& src, const char* maskingKey);
    bool processPackage(const std::string& data, MyOpCode opcode, std::string& res);

    void sendPackage(const char* data, size_t length);

protected:
    std::string                        m_strClientInfo;
    std::string                        m_strUserAgent;

private:
    bool                               m_bUpdateToWebSocket;
    std::map<std::string, std::string> m_mapHttpHeaders;
    std::string                        m_strURL;
    std::string                        m_strParams;
    std::string                        m_strParsedData;
    bool                               m_bClientCompressed;

};