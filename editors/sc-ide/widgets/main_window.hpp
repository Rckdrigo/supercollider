/*
    SuperCollider Qt IDE
    Copyright (c) 2012 Jakob Leben & Tim Blechmann
    http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef SCIDE_WIDGETS_MAIN_WINDOW_HPP_INCLUDED
#define SCIDE_WIDGETS_MAIN_WINDOW_HPP_INCLUDED

#include <QLabel>
#include <QMainWindow>
#include <QProcess>
#include <QSignalMapper>
#include <QStatusBar>

namespace ScIDE {

class Main;
class MultiEditor;
class ToolBox;
class TextFindReplacePanel;
class GoToLineTool;
class PostDock;
class DocumentsDock;
class Document;
class DocumentsDialog;
struct Session;
class StatusLabel;
class StatusClockLabel;

namespace Settings { class Manager; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum ActionRole {
        // File
        Quit = 0,
        DocNew,
        DocOpen,
        DocSave,
        DocSaveAs,
        DocSaveAll,
        DocClose,
        DocCloseAll,
        DocReload,
        ClearRecentDocs,

        // Sessions
        NewSession,
        SaveSessionAs,
        ManageSessions,

        // Edit
        Find,
        Replace,

        // View
        ShowDocList,
        ShowCmdLine,
        ShowGoToLineTool,
        CloseToolBox,
        ShowFullScreen,
        ClearPostWindow,

        LookupDefinition,
        LookupDocumentation,

        // Settings
        ShowSettings,

        // Help
        Help,
        HelpForSelection,

        ActionCount
    };

    explicit MainWindow(Main *);

    QAction *action( ActionRole );

    bool quit();

    void saveWindowState();
    void restoreWindowState();

    static MainWindow *instance() { return mInstance; }

    static bool close( Document * );
    static bool save( Document *, bool forceChoose = false );
    static bool reload( Document * );

public Q_SLOTS:
    void newSession();
    void saveCurrentSessionAs();
    void openSessionsDialog();

    void newDocument();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();
    void saveAllDocuments();
    void reloadDocument();
    void closeDocument();
    void closeAllDocuments();

    void showCmdLine();
    void showFindTool();
    void showReplaceTool();
    void showGoToLineTool();
    void hideToolBox();

    void showSettings();

signals:
    void evaluateCode( const QString &, bool silent = true );


private Q_SLOTS:
    void switchSession( Session *session );
    void saveSession( Session *session );
    void openHelp();
    void helpForCursor();
    void onInterpreterStateChanged( QProcess::ProcessState );
    void onServerStatusReply(int ugens, int synths, int groups, int synthDefs, float avgCPU, float peakCPU);
    void onServerRunningChanged( bool running, QString const & hostName, int port );
    void onQuit();
    void onCurrentDocumentChanged( Document * );
    void onDocumentChangedExternally( Document * );
    void onDocDialogFinished();
    void updateRecentDocsMenu();
    void onRecentDocAction( QAction * );
    void onOpenSessionAction( QAction * );
    void updateWindowTitle();
    void toggleFullScreen();
    void lookupDefinition();
    void lookupDocumentation();
    void showMessage( QString const & string );
    void applySettings( Settings::Manager * );

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createMenus();
    bool promptSaveDocs();
    void updateSessionsMenu();
    QWidget *cmdLine();
    void updateClockWidget( bool isFullScreen );

    Main *mMain;

    QAction * mActions[ActionCount];
    QMenu * mRecentDocsMenu;
    QMenu * mSessionsMenu;

    MultiEditor *mEditors;

    // Tools
    ToolBox *mToolBox;
    QWidget *mCmdLine;
    GoToLineTool *mGoToLineTool;
    TextFindReplacePanel *mFindReplaceTool;

    // Status bar
    QStatusBar  *mStatusBar;
    StatusLabel *mLangStatus;
    StatusLabel *mServerStatus;
    StatusClockLabel *mClockLabel;

    // Docks
    PostDock * mPostDock;
    DocumentsDock *mDocListDock;

    QSignalMapper mCodeEvalMapper;
    DocumentsDialog * mDocDialog;

    static MainWindow *mInstance;
};

class StatusLabel : public QLabel
{
public:
    StatusLabel(QWidget *parent = 0);
    void setBackground(const QBrush &);
    void setTextColor(const QColor &);
};

class StatusClockLabel : public StatusLabel
{
public:
    StatusClockLabel (QWidget * parent = 0);
    ~StatusClockLabel();

private:
    void timerEvent(QTimerEvent *);
    void updateTime();
    int mTimerId;
};

} // namespace ScIDE

#endif
