#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>

#include "eulumdat.h"
#include "maintabwidget.h"

class QAction;
class QMenu;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void documentWasModified();

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void options();
    void validator();    
    void about();
    
    void exportToHTML();
   	void exportToIesna();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    
    void setCurrentFile(const QString &fileName = QString());
    
    QString strippedName(const QString &fullFileName);
    
    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QMenu *exportSubMenu;
    QToolBar *fileToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportToHTMLAct;
    QAction *exportToIesnaAct;    
    QAction *exitAct;
    
    QAction *optionsAct;
    QAction *validatorAct;    
    QAction *aboutAct;
    QAction *aboutQtAct;
   
    QString curFile;    
    bool mods;
    
    void saveFromWidget(void);
    
    void setMaybeSaveTriggers();

public:
    Eulumdat * ldt;
    MainTabWidget *central;
    
    void setValidatorOptions();  
    void loadFile(const QString &fileName);
            
};

#endif
