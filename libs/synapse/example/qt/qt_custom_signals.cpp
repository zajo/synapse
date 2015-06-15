//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This program demonstrates how Boost Synapse can be used to add custom signals to Qt objects
//without having to run the Qt Meta Object Compiler. It creates a QDialog which is populated by
//controls that respond to the custom Boost Synapse signal enable_controls which is emitted by
//the QDialog object when the corresponding QButton is clicked.

#include <boost/synapse/connect.hpp>
#include <boost/bind.hpp>
#define QT_NO_EMIT //Suppress the #define emit from Qt since it clashes with boost::synapse::emit.
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>

namespace synapse=boost::synapse;

namespace
    {
    //Define a custom enable_controls signal which is emitted by the QDialog object to indicate that
    //controls that listen for it should become enabled or disabled.
    typedef struct enable_controls_(*enable_controls)( bool enabled );
    }

int
main( int argc, char const * argv[ ] )
    {
    QApplication app(argc,(char * *)argv);

    //Create a QDialog and populate it with controls.
    boost::shared_ptr<QDialog> qd(new QDialog);
    qd->setGeometry(QRect(QPoint(200,200),QSize(300,100)));
    QVBoxLayout * layout = new QVBoxLayout;
    QCheckBox * c1 = new QCheckBox("Control 1"); layout->addWidget(c1);
    QCheckBox * c2 = new QCheckBox("Control 2"); layout->addWidget(c2);
    QPushButton * b1 = new QPushButton("Enable"); layout->addWidget(b1);
    QPushButton * b2 = new QPushButton("Disable"); layout->addWidget(b2);
    qd->setLayout(layout);
    b1->setMaximumWidth(100);
    b2->setMaximumWidth(100);

    //Emit the enable_controls signal from the QDialog when b1 or b2 is clicked.
    (void) QObject::connect(b1,&QPushButton::clicked,boost::bind(&synapse::emit<enable_controls>,qd.get(),true));
    (void) QObject::connect(b2,&QPushButton::clicked,boost::bind(&synapse::emit<enable_controls>,qd.get(),false));

    //Connect the enable_controls signal from the QDialog to the appropriate QCheckbox member function.
    boost::shared_ptr<synapse::connection> conn1=synapse::connect<enable_controls>(qd,boost::bind(&QCheckBox::setEnabled,c1,_1));
    boost::shared_ptr<synapse::connection> conn2=synapse::connect<enable_controls>(qd,boost::bind(&QCheckBox::setEnabled,c2,_1));

    qd->exec();
    return 0;
    }
