//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//The Qt Wiki contains the following example on creating custom signals and slots:
//https://wiki.qt.io/Qt_for_Beginners#Creating_custom_signals_and_slots.
//It requires running the Qt Meta Object Compiler.

//Below is the same example, modified to use Boost Synapse to implement the
//custom counterReached signal. This approach does not require running the
//Qt Meta Object Compiler.

//The changes made to the original program are marked with //<--

#include <boost/synapse/connect.hpp>
#define QT_NO_EMIT //Suppress the #define emit from Qt since it clashes with synapse::emit.
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

namespace synapse=boost::synapse;

class Window : public QWidget
{
public:
	explicit Window(QWidget *parent = 0);
	signals: //<-- Not needed with Synapse but okay
	typedef struct counterReached_(*counterReached)(); //<-- Was: void counterReached();
private slots: //<-- Not needed with Synapse but okay
	void slotButtonClicked(bool checked);
private:
	int m_counter;
	QPushButton *m_button;
	std::shared_ptr<synapse::connection> c_; //<-- Needed to keep the synapse connection afloat.
};

Window::Window(QWidget *parent) :
	QWidget(parent)
{
	 // Set size of the window
	 setFixedSize(100, 50);

	// Create and position the button
	m_button = new QPushButton("Hello World", this);
	m_button->setGeometry(10, 10, 80, 30);
	m_button->setCheckable(true);

	// Set the counter to 0
	m_counter = 0;

	connect(m_button,&QPushButton::clicked,
		[this]( bool checked )
		{
			slotButtonClicked(checked);
		} ); //<-- Was: connect(m_button, SIGNAL (clicked(bool)), this, SLOT (slotButtonClicked(bool)));
	c_=synapse::connect<counterReached>(this,&QApplication::quit); //<-- Was: connect(this, SIGNAL (counterReached()), QApplication::instance(), SLOT (quit()));
}

void Window::slotButtonClicked(bool checked)
{
	if (checked)
		m_button->setText("Checked");
	else
		m_button->setText("Hello World");
	m_counter ++;
	if (m_counter == 10)
		synapse::emit<counterReached>(this); //<-- Was: emit counterReached();
}

int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	Window window;
	window.show();

	return app.exec();
}
