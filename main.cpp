#include <gtkmm.h>
#include <iostream>
#include <dpp/dpp.h>

void Send_Webhook(const std::string& TokenAPI, const std::string& Message);

class MyWindow : public Gtk::ApplicationWindow
{
public:
    MyWindow();

protected:
    Gtk::Button m_sendButton;
    Gtk::Entry m_textEntry;
    Gtk::Entry m_TokenAPI;
    Gtk::Box m_box;

    void on_send_button_clicked();
    void enable_send_button();
    bool cooldown_handler();
};

MyWindow::MyWindow()
{
    set_title("C-Plus-Webhook");
    set_default_size(1000, 800);
    set_resizable(false);

    m_box.set_orientation(Gtk::Orientation::VERTICAL);
    m_box.set_valign(Gtk::Align::CENTER);
    set_child(m_box);

    m_TokenAPI.set_placeholder_text("Enter API Token here!");
    m_TokenAPI.set_size_request(1000, 25);
    m_TokenAPI.set_hexpand(true);
    m_TokenAPI.set_margin(10);

    m_textEntry.set_placeholder_text("Enter text here");
    m_textEntry.set_size_request(1000, 775);
    m_textEntry.set_hexpand(true);
    m_textEntry.set_margin(10);

    m_sendButton.set_label("Send Message");
    m_sendButton.set_size_request(1000, 50);
    m_sendButton.set_hexpand(true);

    m_sendButton.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_send_button_clicked));

    m_box.append(m_TokenAPI);
    m_box.append(m_textEntry);
    m_box.append(m_sendButton);
}

void MyWindow::on_send_button_clicked()
{
    m_sendButton.set_sensitive(false);

    const std::string& tokenValue = m_TokenAPI.get_text();
    const std::string& messageValue = m_textEntry.get_text();

    Send_Webhook(tokenValue, messageValue);

    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &MyWindow::enable_send_button), 5000); 
}

void MyWindow::enable_send_button()
{
    m_sendButton.set_sensitive(true);
}

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.gtkmm.examples.base");

    return app->make_window_and_run<MyWindow>(argc, argv);
}

void Send_Webhook(const std::string& TokenAPI, const std::string& Message)
{
    try {
        dpp::cluster bot("");
        dpp::webhook wh(TokenAPI);
        bot.execute_webhook(wh, dpp::message(Message));
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
