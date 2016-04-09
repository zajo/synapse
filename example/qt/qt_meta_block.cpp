//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//In Boost Synapse, signals can be disabled by calling the block_signal_from_emitter function.
//When a signal becomes blocked, the meta::emitter emits the meta::blocked<> meta signal.
//By connecting the meta signal we can take action whenever signals are blocked or unblocked.

//This program demonstrates how this can be used to automatically disable/enable buttons
//in response to the button_clicked signal being blocked/unblocked.

#include <boost/synapse/block.hpp>
#include <boost/synapse/blocker.hpp>
#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#define QT_NO_EMIT //Suppress the #define emit from Qt since it clashes with boost::synapse::emit.
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QBoxLayout>

namespace synapse=boost::synapse;

namespace
    {
    //Define a Boost Synapse signal
    typedef struct button_clicked_(*button_clicked)();

    //////////////////////////////////////////////////////////////////////////////////

    //When the button_clicked is being connected, connect the QPushButton::clicked Qt signal to
    //boost::synapse::emit<button_clicked> and store the resulting QMetaObject::Connection object
    //into the boost::synapse::connection object. When disconnecting, disconnect the stored
    //QMetaObject::Connection object.
    void
    handle_button_clicked_connected_meta_signal( synapse::connection & c, unsigned flags )
        {
        if( flags&synapse::meta::connect_flags::connecting )
            {
            boost::shared_ptr<QPushButton> pb=c.emitter<QPushButton>();
            c.user_data = QObject::connect(pb.get(),&QPushButton::clicked,boost::bind(&synapse::emit<button_clicked>,pb.get()));
            }
        else
            QObject::disconnect(*boost::any_cast<QMetaObject::Connection>(&c.user_data));
        }

    //Call the above function whenever a button_clicked signal is connected or disconnected through Boost Synapse.
    boost::shared_ptr<synapse::connection> meta_conn=synapse::connect<synapse::meta::connected<button_clicked> >(
        synapse::meta::emitter(),
        &handle_button_clicked_connected_meta_signal );

    //////////////////////////////////////////////////////////////////////////////////

    //Whenever the button_clicked signal is blocked, disable the button. Enable when unblocked.
    void
    handle_button_clicked_blocked_meta_signal( synapse::blocker & b, bool blocked )
        {
        b.emitter<QPushButton>()->setDisabled(blocked);
        }

    //Call the above function whenever a button_clicked signal is blocked or unblocked through Boost Synapse.
    boost::shared_ptr<synapse::connection> meta_block=synapse::connect<synapse::meta::blocked<button_clicked> >(
        synapse::meta::emitter(),
        &handle_button_clicked_blocked_meta_signal );

    //This function is connected to the appropriate Qt signals and it blocks or unblocks the button_clicked signal.
    void
    block_unblock( boost::shared_ptr<synapse::blocker> & blk, boost::weak_ptr<QPushButton> const & btn, bool block )
        {
        if( block )
            blk=synapse::block<button_clicked>(btn);
        else
            blk.reset();
        }
    }

int
main( int argc, char const * argv[ ] )
    {
    QApplication app(argc,(char * *)argv);

    //Create a QDialog with a QPushButton and a check QCheckBox.
    boost::shared_ptr<QDialog> qd(new QDialog);
    qd->setGeometry(QRect(QPoint(200,200),QSize(300,100)));
    QVBoxLayout * layout = new QVBoxLayout;
    QPushButton * pb=new QPushButton("OK"); layout->addWidget(pb);
    QCheckBox * cb1=new QCheckBox("Disabler 1"); layout->addWidget(cb1);
    QCheckBox * cb2=new QCheckBox("Disabler 2"); layout->addWidget(cb2);
    qd->setLayout(layout);
    pb->setMaximumWidth(100);

    //Make a weak_ptr to the QPushButton using shared_ptr aliasing constructor.
    boost::weak_ptr<QPushButton> pbw=boost::shared_ptr<QPushButton>(qd,pb);

    //accept() the QDialog when pb is clicked.
    boost::shared_ptr<synapse::connection> c=synapse::connect<button_clicked>(pbw,boost::bind(&QDialog::accept,qd.get()));

    //Block/unblock the button_clicked Boost Synapse signal when either checkbox is toggled.
    //This simulates two conditions which independently block the button_clicked signal.
    //The handle_button_clicked_blocked_meta_signal is called when transitioning from
    //zero checkboxes being checked to a checkbox being checked, and when transitioning from
    //from at least one checkbox being checked to zero checkboxes being checked.
    boost::shared_ptr<synapse::blocker> blk1;
    boost::shared_ptr<synapse::blocker> blk2;
    (void) QObject::connect(cb1,&QCheckBox::stateChanged,boost::bind(&block_unblock,boost::ref(blk1),pbw,_1));  
    (void) QObject::connect(cb2,&QCheckBox::stateChanged,boost::bind(&block_unblock,boost::ref(blk2),pbw,_1));  

    qd->exec();
    return 0;
    }
