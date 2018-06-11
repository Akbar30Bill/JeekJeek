#include <iostream>
#include <vector>
#include <string>
#include "sha512.h"

using namespace std;

int global_id = 0;

/*enumz*/
enum cmd {_text , _tag , _mention , _publish , _abort , _def};
enum like_type { _like , _unlike };
enum follow_type { _follow , _unfollow };
enum error_type { _not_signedin , _wrong_input , _wrong_username , _wrong_user_pass , _wrong_id };

/*readding functions*/
void vec_equaler(vector<string>& first , vector<string>& second)
{
	for ( int i = 0 ; i < first.size() ; i++ )
		first.erase(first.begin());
	for ( int j = 0 ; j < second.size() ; j++ )
		first.push_back(second[j]);
	return;
}
cmd get_cmd (string n)
{
	if ( n[0] == 't' && n[1] == 'e' && n[2] == 'x' && n[3] == 't' )
		return _text;
	else if ( n[0] == 't' && n[1] == 'a' && n[2] == 'g' )
		return _tag;
	else if ( n[0] == 'm' && n[1] == 'e' && n[2] == 'n' && n[3] == 't' && n[4] == 'i' && n[5] == 'o' && n[6] == 'n' )
		return _mention;
	else if ( n[0] == 'p' && n[1] == 'u' && n[2] == 'b' && n[3] == 'l' && n[4] == 'i' && n[5] == 's' && n[6] == 'h' )
		return _publish;
	else if ( n[0] == 'a' && n[1] == 'b' && n[2] == 'o' && n[3] == 'r' && n[4] == 't' )
		return _abort;
	else
		return _def;
}
string scan_txt (string n)
{
	string temp = "";
	for ( int i = 0 ; i < n.size() ; i++ )
	{
		if ( n[i] == ' ' )
		{
			for ( int j = i + 1 ; j < n.size() ; j++ )
			{
				temp.push_back(n[j]);
			}
			return temp;
		}
	}
	throw error_type (_wrong_input);
	return temp;
}
string scan_mention (string n)
{
	string temp = "";
	for (int i = 0 ; i < n.size() ; i++)
	{
		if ( n[i] == ' ' )
		{
			for ( int j = i + 1 ; j < n.size() ; j++ )
			{
				if (n[j] == ' ')
					return temp;
				temp.push_back(n[j]);
			}
			return temp;
		}
	}
	temp = "";
	return temp;
}
string scan_tag (string n)
{
	string temp = "";
	for (int i = 0 ; i < n.size() ; i++)
	{
		if ( n[i] == ' ' )
		{
			for ( int j = i + 1 ; j < n.size() ; j++ )
			{
				if (n[j] == ' ')
					return temp;
				temp.push_back(n[j]);
			}
			return temp;
		}
	}
	temp = "";
	return temp;
}
vector<string> scan_main_cmd()
{
	string command;
	vector<string> command_vector;
	getline(cin , command);
	string temp;
	for ( int i = 0 ; i < command.size() ; i++ )
	{
		if (command[i] == ' ')
		{
			command_vector.push_back(temp);
			temp = "";
		}
		else
			temp.push_back(command[i]);
	}
	command_vector.push_back(temp);
	return command_vector;
}
void terminate_string(string& s)
{
	for ( int i = 0 ; i < s.size() ; i++ )
		s[i] = 0;
	return;
}

/*clases*/
class Reply {
private:
	int				reply_id;
	string			reply_txt;
	string			user_who_replyed;
	vector<Reply*>	reply_reply;
public:
	int get_reply_id() {return reply_id;}
	Reply(string txt , string username)
	{
		reply_txt = txt;
		user_who_replyed = username;
		global_id++;
		reply_id = global_id;
	}
	string reply (int _reply_id , string _reply_txt , string username)
	{
		string notification = "";
		if (reply_id == _reply_id)
		{
			Reply* new_reply = new Reply(_reply_txt , username);
			reply_reply.push_back(new_reply);
			notification = username + " replied " + to_string(reply_id);
			return notification;
		}
		else
		{
			for (int i = 0 ; i < reply_reply.size() ; i++)
			{
				notification = reply_reply[i]->reply(_reply_id , _reply_txt , username);
				if ( notification != "" )
					return notification;
			}
		}
	}
	void show_reply(int rp_id , string username)
	{
		if ( reply_id == rp_id )
		{
			cout << username << endl;
			cout << reply_txt << endl;
			cout << "Replies:" << endl;
			for ( int i = 0 ; i < reply_reply.size() ; i++ )
				cout << reply_reply[i]->get_reply_id() << endl;
		}
		else
		{
			for ( int i = 0 ; i < reply_reply.size() ; i++ )
				show_reply(rp_id , username);
		}
		return;
	}
};
class Comment {
private:
	int 			comment_id;
	string			user_who_commented;
	string			comment_txt;
	vector<Reply*>	comment_reply;
public:
	Comment(string cm_txt , string username)
	{
		comment_txt = cm_txt;
		global_id++;
		comment_id = global_id;
		user_who_commented = username;
	}
	int get_id() {return comment_id;}
	string reply(int reply_id , string reply_txt , string username)
	{
		string notification;
		if ( comment_id == reply_id )
		{
			Reply* new_reply = new Reply(reply_txt , username);
			comment_reply.push_back(new_reply);
			notification = username + " replied " + to_string(reply_id);
			return notification;
		}
		else
		{
			for (int i = 0 ; i < comment_reply.size() ; i++)
			{
				notification = comment_reply[i]->reply(reply_id , reply_txt , username);
				if ( notification != "" )
					return notification;
			}
		}
	}
	void show_comment(int cm_id , string username)
	{
		if (comment_id == cm_id)
		{
			cout << username 	<< endl;
			cout << comment_txt << endl;
			cout << "Replies:" 	<< endl;
			for ( int i = 0 ; i < comment_reply.size() ; i++ )
				cout << comment_reply[i]->get_reply_id() << endl;
		}
		return;
	}
	void show_reply(int rp_id , string username)
	{
		for ( int i = 0 ; i < comment_reply.size() ; i++ )
			comment_reply[i]->show_reply(rp_id , username);
		return;
	}
};
class Jeek {
private:
	int					jeek_id;
	string				jeek_txt;
	vector<string>		hash_tag;
	vector<string>		mentioned_user;
	vector<string>		liked_user;
	// vector<string>		disliked_user;
	vector<string>		rejeeked_user;
	vector<Comment*>	jeek_comment;
public:
	Jeek () {}
	vector<string> get_mentioned_users() {return mentioned_user;}
	int get_jeek_id() {return jeek_id;}
	void rejeek_plus_plus (string username)
	{
		rejeeked_user.push_back(username);
		return;
	}
	void rejeek(Jeek* j)
	{
		global_id++;
		jeek_id 		= global_id;
		jeek_txt		= "Rejeek: " + j->jeek_txt;
		hash_tag		= j->hash_tag;
		mentioned_user	= j->mentioned_user;
		return;
	}
	void print_jeek()
	{
		cout << jeek_txt << endl;
		for ( int i = 0 ; i < hash_tag.size() ; i++ )
			cout << "#" << hash_tag[i] << endl;
		for ( int i = 0 ; i < mentioned_user.size() ; i++ )
			cout << "@" << mentioned_user[i] << endl;
		cout << "Likes "	<< liked_user.size()	<< endl;
		cout << "Rejeeks "	<< rejeeked_user.size()	<< endl;
		cout << "comments:" << endl;
		for ( int i = 0 ; i < jeek_comment.size() ; i++ )
			cout << jeek_comment[i]->get_id() << endl;
	}
	bool run_jeek()
	{
		string input = "";
		string line = "\n";
		while ( line != "")
		{
			getline(cin , line);
			switch (get_cmd(line))
			{
			case _text:
			{
				jeek_txt = scan_txt(line);
				break;
			}
			case _tag:
			{
				hash_tag.push_back(scan_tag(line));
				break;
			}
			case _mention:
			{
				mentioned_user.push_back(scan_mention(line));
				break;
			}
			case _publish:
			{
				global_id++;
				jeek_id = global_id;
				return 1;
				break;
			}
			case _abort:
			{
				return 0;
				break;
			}
			default:
			{
				throw error_type(_wrong_input);
			}
			}
		}
	}
	void print_searched_jeek(string username)
	{
		cout << jeek_id << " " << username << endl;
		cout << jeek_txt << endl;
		return;
	}
	void print_tag_matching_jeeks(string tag , string username)
	{
		for ( int i = 0 ; i < hash_tag.size() ; i++ )
		{
			if ( hash_tag[i] == tag )
			{
				cout << jeek_id << " " << username << endl;
				cout << jeek_txt << endl << endl;
			}
		}
		return;
	}
	string comment(int j_id , string cm_txt , string username)
	{
		string notification = "";
		if ( jeek_id == j_id )
		{
			notification = username + " commented on " + to_string(j_id);
			Comment* new_comment = new Comment(cm_txt , username);
			jeek_comment.push_back(new_comment);
			return notification;
		}
		return notification;
	}
	string reply(int id , string reply_txt , string username )
	{
		string notification = "";
		for ( int i = 0 ; i < jeek_comment.size() ; i++ )
		{
			notification = jeek_comment[i]->reply(id , reply_txt , username);
			if ( notification != "" )
				return notification;
		}
	}
	void show_comment(int cm_id , string username)
	{
		for ( int i = 0 ; i < jeek_comment.size() ; i++ )
			jeek_comment[i]->show_comment(cm_id , username);
		return;
	}
	void show_reply(int rp_id , string username)
	{
		for ( int i = 0 ; i < jeek_comment.size() ; i++ )
			jeek_comment[i]->show_reply(rp_id , username);
		return;
	}
	string like(int id , like_type type , string _username)
	{
		string notification = "";
		if ( id == jeek_id )
		{
			for ( int i = 0 ; i < liked_user.size() ; i++ )
			{
				if ( _username == liked_user[i] )
				{
					if ( type == _unlike )
					{
						liked_user.erase(liked_user.begin() + i - 1);
						notification = _username + "disliked" + to_string(id);
					}
					return notification;
				}
			}
			if ( type == _like )
			{
				liked_user.push_back(_username);
				notification = _username + "liked" + to_string(id);
			}
			return notification;
		}
	}
};
class User {
private:
	string					display_name;
	string					username;
	string					password;
	vector<Jeek*>			jeek;
	vector<string>			user_notification;
	vector<string>			following_users;
public:
	User( string _uname , string _display_name , string _pass ) : username(_uname) , display_name(_display_name) , password(sha512(_pass)) {}
	string get_username() {return username;}
	void add_notification(string notification)
	{
		if ( notification == "" )
			return;
		user_notification.push_back(notification);
		return;
	}
	void rejeek_this ( Jeek* j )
	{
		j->rejeek_plus_plus(username);
		Jeek* new_rejeek = new Jeek();
		new_rejeek->rejeek(j);
		jeek.push_back(new_rejeek);
	}
	void show_jeek(int j_id)
	{
		for ( int i = 0 ; i < jeek.size() ; i++ )
		{
			if ( jeek[i]->get_jeek_id() == j_id )
				jeek[i]->print_jeek();
		}
	}
	bool check_password(string _pswrd)
	{
		if ( password == _pswrd )
		{
			terminate_string(_pswrd);
			return 1;
		}
		else
			return 0;
	}
	vector<string> new_jeek ()
	{
		vector<string> default_vec(0);
		Jeek* new_jeek = new Jeek();
		if (new_jeek->run_jeek())
		{
			jeek.push_back(new_jeek);
			return new_jeek->get_mentioned_users();
		}
		return default_vec;
	}
	Jeek* return_jeek ( int j_id )
	{
		for ( int i = 0 ; i < jeek.size() ; i++ )
		{
			if (jeek[i]->get_jeek_id() == j_id)
				return jeek[i];
		}
		return NULL;
	}
	void print_all_jeeks()
	{
		for (int i = 0 ; i < jeek.size() ; i++)
			jeek[i]->print_searched_jeek(display_name);
	}
	void print_tag_matching_jeeks(string tag)
	{
		for ( int i = 0 ; i < jeek.size() ; i++ )
			jeek[i]->print_tag_matching_jeeks(tag , display_name);
		return;
	}
	void comment (int jeek_id , string cm_txt)
	{
		for ( int i = 0 ; i < jeek.size() ; i++ )
			add_notification(jeek[i]->comment(jeek_id , cm_txt , display_name));
		return;
	}
	void reply(int id , string reply_txt)
	{
		string notification = "";
		for ( int i = 0 ; i < jeek.size() ; i++ )
			add_notification(jeek[i]->reply(id , reply_txt , display_name));
		return;
	}
	void show_comment(int cm_id)
	{
		for (int i = 0 ; i < jeek.size() ; i++ )
			jeek[i]->show_comment(cm_id , display_name);
		return;
	}
	void show_reply(int rp_id)
	{
		for ( int i = 0 ; i < jeek.size() ; i++ )
			jeek[i]->show_reply(rp_id , display_name);
		return;
	}
	void like (int id , like_type type)
	{
		for ( int i = 0 ; i < jeek.size() ; i++ )
			add_notification(jeek[i]->like(id , type , username));
		return;
	}
	void follow(string username , follow_type type)
	{
		for ( int i = 0 ; i < following_users.size() ; i++ )
			if ( username == following_users[i] )
				return;
		if ( type == _follow )
		{
			following_users.push_back(username);
			return;
		}
		else if ( type == _unfollow )
		{
			for ( int i = 0 ; i < following_users.size() ; i++ )
			{
				if ( username == following_users[i] )
					following_users.erase(following_users.begin() + i - 1);
			}
		}
	}
	void show_notifications()
	{
		for ( int i = 0 ; i < user_notification.size() ; i++ )
			cout << user_notification[i] << endl;
		return;
	}
	void add_jeek_notification(string jeeked_nigga)
	{
		if ( jeeked_nigga == username )
			return;
		for ( int i = 0 ; i < following_users.size() ; i++ )
		{
			if ( jeeked_nigga == following_users[i] )
			{
				add_notification (jeeked_nigga + " jeeked " + to_string(global_id));
				return;
			}
		}
	}
};
class Network {
private:
	vector<User*>	user;
	User*			signedin_user;
public:
	Network()
	{
		signedin_user = NULL;
	}
	User* login ( string username , string password )
	{
		if ( signedin_user != NULL )
			return NULL;
		for ( int i = 0 ; i < user.size() ; i++ )
		{
			if (username == user[i]->get_username())
			{
				if (user[i]->check_password(sha512(password)))
				{
					signedin_user = user[i];
					terminate_string(password);
				}
			}
		}
		if ( signedin_user == NULL )
			throw error_type(_wrong_user_pass);
		return signedin_user;
	}
	bool signup ( string username , string display_name , string password )
	{
		for ( int i = 0 ; i < user.size() ; i++ )
		{
			if (user[i]->get_username() == username)
				throw error_type(_wrong_username);
		}
		User* new_user = new User(username , display_name , password);
		user.push_back(new_user);
		return 1;
	}
	void logout()
	{
		signedin_user = NULL;
	}
	/*--------------------------------------------------*/
	void new_jeek()
	{
		if (signedin_user == NULL)
			throw error_type(_not_signedin);
		vector<string> mentioned_user;
		mentioned_user = signedin_user->new_jeek();
		if ( mentioned_user.size() < 1 )
			return;
		else
			for ( int i = 0 ; i < mentioned_user.size() ; i++ )
				for ( int j = 0 ; j < user.size() ; j++ )
					if ( user[j]->get_username() == mentioned_user[i] )
					{
						string notification = signedin_user->get_username() + " mentioned you in " + to_string(global_id);
						user[j]->add_notification(notification);
					}
		for ( int i = 0 ; i < user.size() ; i++ )
			user[i]->add_jeek_notification(signedin_user->get_username());
		return;
	}
	void show_jeek(int j_id)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
		{
			user[i]->show_jeek(j_id);
		}
		return;
	}
	void rejeek(int j_id)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
		{
			if ( user[i]->return_jeek(j_id) != NULL )
				signedin_user->rejeek_this(user[i]->return_jeek(j_id));
		}
		return;
	}
	void search_by_id (string username)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
		{
			if ( user[i]->get_username() == username )
				user[i]->print_all_jeeks();
		}
	}
	void search_by_tag (string tag)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
			user[i]->print_tag_matching_jeeks(tag);
		return;
	}
	void comment (int jeek_id , string comment)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
			user[i]->comment(jeek_id , comment);
		return;
	}
	void reply (int id , string reply_txt)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
			user[i]->reply(id , reply_txt);
	}
	void show_comment(int cm_id)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
			user[i]->show_comment(cm_id);
		return;
	}
	void show_reply(int rp_id)
	{
		for ( int i = 0 ; i < user.size() ; i++ )
			user[i]->show_reply(rp_id);
		return;
	}
	void like (int jeek_id , like_type type)
	{
		for (int i = 0 ; i < user.size() ; i++)
			user[i]->like(jeek_id , type);
		return;
	}
	void follow (string username , follow_type type)
	{
		if ( signedin_user->get_username() == username )
			return;
		for ( int i = 0 ; i < user.size() ; i++ )
			if ( user[i]->get_username() == username )
				signedin_user->follow(username , type);
		return;
	}
	void show_notifications()
	{
		if (signedin_user == NULL)
			throw error_type(_not_signedin);
		else
			signedin_user->show_notifications();
	}
	void whoami()
	{
		if ( signedin_user == NULL )
			throw error_type(_not_signedin);
		cout << signedin_user->get_username() << endl;
		return;
	}
};

void run()
{
	Network network;
	while (true)
	{
		try
		{
			vector<string> command_vector = scan_main_cmd();

			if (command_vector[0] == "signup")
				network.signup(command_vector[1] , command_vector[2] , command_vector[3]);
			else if ( command_vector[0] == "login" )
				network.login(command_vector[1] , command_vector[2]);
			else if ( command_vector[0] == "logout" )
				network.logout();
			else if ( command_vector[0] == "jeek" )
				network.new_jeek();
			else if ( command_vector[0] == "showJeek" )
				network.show_jeek(stoi(command_vector[1]));
			else if ( command_vector[0] == "search" )
			{
				if ( command_vector[1][0] == '@' )
				{
					command_vector[1].erase(command_vector[1].begin());
					network.search_by_id(command_vector[1]);
				}
				else if ( command_vector[1][0] == '#' )
				{
					command_vector[1].erase(command_vector[1].begin());
					network.search_by_tag(command_vector[1]);
				}
				else
					throw error_type(_wrong_input);
			}
			else if ( command_vector[0] == "comment" )
				network.comment(stoi(command_vector[1]) , command_vector[2]);
			else if ( command_vector[0] == "reply" )
				network.reply(stoi(command_vector[1]) , command_vector[2]);
			else if ( command_vector[0] == "rejeek" )
				network.rejeek(stoi(command_vector[1]));
			else if ( command_vector[0] == "showJeek" )
				network.show_jeek(stoi(command_vector[1]));
			else if ( command_vector[0] == "showComment" )
				network.show_comment(stoi(command_vector[1]));
			else if ( command_vector[0] == "showReply" )
				network.show_reply(stoi(command_vector[1]));
			else if ( command_vector[0] == "like" )
				network.like(stoi(command_vector[1]) , _like);
			else if ( command_vector[0] == "dislike" )
				network.like(stoi(command_vector[1]) , _unlike);
			else if ( command_vector[0] == "follow" )
				network.follow(command_vector[1] , _follow);
			else if ( command_vector[0] == "unfollow" )
				network.follow(command_vector[1] , _unfollow);
			else if ( command_vector[0] == "notification" )
				network.show_notifications();
			else if ( command_vector[0] == "whoami" )
				network.whoami();
			else if ( command_vector[0] == "suka" )
				cout << "blin you found an easter egg \nany ways your input method was wrong \nIDI NAHUI BLYAT \\M/" << endl;
			else if ( command_vector[0] == "exit" )
				return;
			else
				throw error_type(_wrong_input);
		}
		catch (error_type error)
		{
			if ( error == _wrong_input )
				cerr << "wrong input method" << endl;
			else if ( error == _not_signedin )
				cerr << "you have to signin first" << endl;
			else if ( error == _wrong_username )
				cerr << "wrong username" << endl;
			else if ( error == _wrong_user_pass )
				cerr << "username or password was wrong" << endl;
		}
	}
}
