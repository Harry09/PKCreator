#pragma once
#include <QPlainTextEdit>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

private:
	QWidget *m_pLineNumberArea;

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void SetupEditor();

public:
	explicit CodeEditor(QWidget *parent);
	~CodeEditor();

	void LineNumberAreaPaintEvent(QPaintEvent *event) const;
	int LineNumberAreaWidth() const;

private slots:
	void UpdateLineNumberAreaWidth(int newBlockCount);
	void HighlightCurrentLine();
	void UpdateLineNumberArea(const QRect &, int);
};

