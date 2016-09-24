//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This program demonstrates how meta signals can be used to connect native Qt signals
//through the Boost Synapse interface.

//When the user calls synapse::connect<button_clicked> passing a QPushButton,
//Boost Synapse emits the synapse::meta::connected<button_clicked> signal from the
//synapse::meta::emitter. The handle_button_clicked_connected_meta_signal function
//responds to this meta signal by connecting (or disconnecting) the Qt-native
//QPushButton::clicked signal to the synapse::emit<button_clicked> function.

#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#define QT_NO_EMIT //Suppress the #define emit from Qt since it clashes with synapse::emit.
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

namespace synapse=boost::synapse;

namespace
    {
    //Define a Boost Synapse signal
    typedef struct button_clicked_(*button_clicked)();

    //Connect the QPushButton::clicked Qt signal to synapse::emit<button_clicked>.
    //Store the QMetaObject::Connection object into the synapse::connection object.
    boost::shared_ptr<synapse::connection> meta_conn=synapse::connect<synapse::meta::connected<button_clicked> >(
        synapse::meta::emitter(),
        [ ]( synapse::connection & c, unsigned flags )
            {
            if( flags&synapse::meta::connect_flags::connecting )
                {
                QPushButton * pb=c.emitter<QPushButton>().get();
                c.set_user_data( QObject::connect( pb, &QPushButton::clicked, [pb]() { synapse::emit<button_clicked>(pb); } ) );
                }
            else
                QObject::disconnect(*c.get_user_data<QMetaObject::Connection>());
            } );
    }

int
main( int argc, char const * argv[ ] )
    {
    QApplication app(argc,(char * *)argv);
    QDialog qd;
    qd.setGeometry(QRect(QPoint(200,200),QSize(300,100)));
    boost::shared_ptr<QPushButton> pb(new QPushButton("OK",&qd));

    //accept() the QDialog when pb is clicked.
    boost::shared_ptr<synapse::connection> c=synapse::connect<button_clicked>( pb, [&qd]() { qd.accept(); } );

    qd.exec();
    return 0;
    }
