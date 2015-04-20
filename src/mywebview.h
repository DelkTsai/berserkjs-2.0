#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

/**
 * MyWebView 类
 * 用于扩展QWebEngineView类
 * 包装出基于QWebEngineView的JS相关实现方法
 * @author Tapir
 * @date   2014-04-15
 */

#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtScript/QScriptClass>
#include <QTimer>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

#include "networkaccessmanager.h"
#include "cookiejar.h"
#include "scriptbinding.h"
#include "monitordatamap.h"
#include "mywebpage.h"
#include "firstscreen.h"

class MyWebView : public QWebView
{
    Q_OBJECT
public:
    MyWebView();
    ~MyWebView();
    void setAppScriptEngine(ScriptBinding* scriptBinding);
    void sendPageMessage(QString message, QString type);
    void sendSignal(QString signal, QString value = "");


    Q_INVOKABLE QScriptValue open(QScriptValue url =  QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue execScript(QScriptValue scriptFunc, QScriptValue args = QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue setTimeout(QScriptValue scriptFunc, QScriptValue timeout = 1);
    Q_INVOKABLE QScriptValue setInterval(QScriptValue scriptFunc, QScriptValue timeout = 1);
    Q_INVOKABLE QScriptValue clearTimeout(QScriptValue timeId);
    Q_INVOKABLE QScriptValue clearInterval(QScriptValue timeId);
    Q_INVOKABLE QScriptValue addEventListener(QScriptValue eventName, QScriptValue scriptFunc);
    Q_INVOKABLE QScriptValue removeEventListener(QScriptValue eventName, QScriptValue scriptFunc);
    Q_INVOKABLE QScriptValue removeAllEventListener(QScriptValue eventName =  QScriptValue(""));
    Q_INVOKABLE QScriptValue saveImage(QScriptValue path, QScriptValue type = QScriptValue("JPG"),
                                     QScriptValue quality = QScriptValue(-1),
                                     QScriptValue rect = QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue savePdf(QScriptValue path);

    Q_INVOKABLE QScriptValue elementRects(QScriptValue cssSelector);
    Q_INVOKABLE QScriptValue viewport();
    Q_INVOKABLE QScriptValue contentRect();
    Q_INVOKABLE QScriptValue dataURIFromRect(QScriptValue rect = QScriptValue::UndefinedValue,
                                             QScriptValue type = QScriptValue::UndefinedValue,
                                             QScriptValue quality = QScriptValue(-1));
    Q_INVOKABLE QScriptValue setViewport(QScriptValue size);
    Q_INVOKABLE QScriptValue getUrl();
    Q_INVOKABLE QScriptValue netListener(QScriptValue enabled = QScriptValue(true));
    Q_INVOKABLE QScriptValue cookiesFromUrl(QScriptValue url = QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue setCookiesFromUrl(QScriptValue cookie,
                                               QScriptValue url = QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue cookieObject(QScriptValue url = QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue setCookie(QScriptValue cookieObject = QScriptValue::UndefinedValue);

    Q_INVOKABLE QScriptValue removeCookie(QScriptValue name = QScriptValue::UndefinedValue,
                                          QScriptValue domain = QScriptValue::UndefinedValue);

    Q_INVOKABLE QScriptValue clearCookie();

    Q_INVOKABLE QScriptValue setProxy(QScriptValue host,
                                      QScriptValue type = QScriptValue::UndefinedValue,
                                      QScriptValue userName = QScriptValue::UndefinedValue,
                                      QScriptValue password = QScriptValue::UndefinedValue);
    Q_INVOKABLE QScriptValue clearProxy();
    Q_INVOKABLE QScriptValue useSystemProxy(QScriptValue index = QScriptValue(0));
    Q_INVOKABLE QScriptValue setUserAgent(QScriptValue userAgent);
    Q_INVOKABLE QScriptValue userAgent();
    Q_INVOKABLE QScriptValue defaultUserAgent();
    Q_INVOKABLE QScriptValue setDetectionRects(QScriptValue rects, QScriptValue sameRate = QScriptValue(0.95));
    Q_INVOKABLE QScriptValue hasDetectionRects();
    Q_INVOKABLE QScriptValue clearDetectionRects();

    Q_INVOKABLE QScriptValue setPageZoom(QScriptValue zoom);
    Q_INVOKABLE QScriptValue pageZoom();
    Q_INVOKABLE QScriptValue setPageScroll(QScriptValue point);
    Q_INVOKABLE QScriptValue pageScroll();
    Q_INVOKABLE QScriptValue pageHTML();
    Q_INVOKABLE QScriptValue setPageHTML(QScriptValue html = QScriptValue(""));
    Q_INVOKABLE QScriptValue pageText();
    Q_INVOKABLE QScriptValue setUploadFile(QScriptValue selector, QScriptValue path, QScriptValue index = QScriptValue(0));
    Q_INVOKABLE QScriptValue setMaxPagesInCache(QScriptValue num = QScriptValue(100));
    Q_INVOKABLE QScriptValue maxPagesInCache();
    Q_INVOKABLE QScriptValue clearAllPagesInCache();


    bool clipRenderToImage(QString path, QString type = "JPG",
                       int quality = 60,  QRect clipRect = QRect());
    QImage renderToImage();

    void fixClipRectToRenderRect(QRect* clipRect, QImage* image);

private:

    typedef struct {
        QScriptValue func;
        QScriptValue activationObject;
        QScriptValue thisObject;
    } ContextInfo;

    typedef struct {
        QTimer* timer;
        ContextInfo info;
    } TimerInfo;

    FirstScreen* firstScreen;

    MyWebPage* myPage;
    QWebFrame* myFrame;
    NetworkAccessManager *newManager;
    CookieJar* cookieJar;
    ScriptBinding* script;
    QScriptEngine* appEngine;
    QString webViewNamespace;

    bool hasFirstPaintFinished;
    bool hasDOMLoaded;

    qint64 loadStartedTime;

    QMap<QString, QList<ContextInfo>* > eventNameMap;
    QList<ContextInfo> javaScriptWindowObjectClearedFunc;
    QList<ContextInfo> loadFinishedFunc;
    QList<ContextInfo> initialLayoutCompletedFunc;
    QList<ContextInfo> pageChangedFunc;
    QList<ContextInfo> contentsSizeChangedFunc;
    QList<ContextInfo> iconChangedFunc;
    QList<ContextInfo> loadStartedFunc;
    QList<ContextInfo> titleChangedFunc;
    QList<ContextInfo> urlChangedFunc;
    QList<ContextInfo> repaintRequestedFunc;
    QList<ContextInfo> geometryChangeRequestedFunc;
    QList<ContextInfo> loadProgressFunc;
    QList<ContextInfo> pageMessageFunc;
    QList<ContextInfo> pageFirstPaintFinishedFunc;
    QList<ContextInfo> pageFirstScreenFinishedFunc;
    QList<ContextInfo> requestFinishedFunc;
    QList<ContextInfo> requestStartFunc;
    QList<ContextInfo> pageConsoleMessageFunc;
    QList<ContextInfo> pagePromptFunc;
    QList<ContextInfo> pageConfirmFunc;
    QList<ContextInfo> pageAlertFunc;
    QList<ContextInfo> printRequestedFunc;
    QList<ContextInfo> scrollRequestedFunc;
    QList<ContextInfo> selectionChangedFunc;
    QList<ContextInfo> windowCloseRequestedFunc;
    QList<ContextInfo> statusBarMessageFunc;
    QList<ContextInfo> DOMContentLoadedFunc;

    QMap<QString, TimerInfo> timeEventMap;
    void scriptError();
    void normalFireEvent(QList<ContextInfo> eventHandleList);
    void initEventNameMap();
    void initEvents();
    void initAppScriptEngine(ScriptBinding* scriptBinding);
    void setAppScriptWebViewObject();
    QScriptValue getWebViewObjcet();

    // 移除无用方法定义
    //void setCustomNetWorkAccessManager();
    //void resetNetWorkAccessManager();

    QString sizeToJson(const QSize & size);
    QString rectToJson(const QRect & rect);


    QList<QPoint> customFirstScreenDetectionPoints;
    float customFirstScreenDetectionPointsSameRate;
    void rectToPoints(QRect* rect, QList<QPoint>* points);
    void fixRectToViewport(QRect* rect,  QSize *viewport);

    QScriptValue rectToScriptObject(const QRect & rect);
    QScriptValue sizeToScriptObject(const QSize & size);
    QString jsonStringify(QScriptValue scriptObject);
    QScriptValue jsonParse(QScriptValue jsonString);
    QScriptValue timer(QScriptValue scriptFunc, QScriptValue timeout, bool singleShot);
    QScriptValue clearTimer(QScriptValue timeId);

    QUrl checkURL(QScriptValue url);

    void onPageMessage(QString wparam, QString lparam);



signals:
    // 使用自定义信号构造一个 DOMContentLoaded 事件
    void DOMContentLoaded(int timeout, QString url);

private slots:
    void onJavaScriptWindowObjectCleared();
    void onLoadFinished(bool ok);
    void onInitialLayoutCompleted();
    void onPageChanged();
    void onContentsSizeChanged(const QSize & size);
    void onIconChanged();
    void onLoadStarted();
    void onTitleChanged(const QString & title);
    void onUrlChanged(const QUrl & url);

    void onPrintRequested (QWebFrame * frame);
    void onScrollRequested (int dx, int dy, const QRect & rectToScroll);
    void onSelectionChanged();
    void onWindowCloseRequested();
    void onStatusBarMessage(const QString & text);

    void onRepaintRequested(const QRect & dirtyRect);
    void onGeometryChangeRequested(const QRect & geom);
    void onLoadProgress(int progress);
    void onTimeout();

    void onRequestStart(QString url);
    void onRequestFinished(QString url);

    void onPageConsoleMessage(QString message, int lineNumber, QString sourceID);
    void onPagePrompt(QString msg, QString defaultValue);
    void onPageConfirm(QString msg);
    void onPageAlert(QString msg);

    void onOpenUrl(QUrl targetURL);


    // 为首屏与首次渲染准备的综合事件处理
    void onFirst_UrlChanged(const QUrl & url);
    void onFirst_RepaintRequested(const QRect & dirtyRect);
    void onFirstScreenRenderTimeout(int timeout, QString url);
    // 为 DOMContentLoaded 准备的综合事件处理
    void onDOMLoaded_UrlChanged(const QUrl & url);
    void onDOMLoaded_TitleChanged(const QString & title);
    void onDOMContentLoaded(int timeout, QString url);
};

#endif // MYWEBVIEW_H
