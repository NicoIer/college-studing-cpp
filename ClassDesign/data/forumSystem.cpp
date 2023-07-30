#include "forumSystem.h"

/*****************************************************************************************************/
bool forumSystem::read()
{//��ȡ��̳��Ϣ
    fstream file;
    file.open(USER_PATH);//�ȶ��û���Ϣ �� users
    
    while (file.peek() != EOF)
    {
        string u;
        file >> u;
        users.insert(toUser(u));
    }
    file.close();

    //��ȡ������Ϣ
    //�ȶ�ȡĿ¼�µ���������·��
    StringList roads;
    getFiles(CONTENT_PATH, roads);

        //���Ӹ�ʽ
        //[�����ཻ]���������� 
        //*floor* name* time* text*

    for (auto& road : roads)
    {//��ÿһ������

        file.open(road);

        article tmp;
        //��һ���Ǳ���
        file >> tmp.title;

        //�ڶ�����һ¥�������ߣ�  ������Ϣ��ȡ
        string str;
        file >> str;
        content c(toContent(str));
        tmp.publisher = c.publisher;
        tmp.time = c.time;
        tmp.contents.push_back(c);

        while (file.peek() != EOF)
        {//����¥���ȡ
            file >> str;
            tmp.contents.push_back(toContent(str));
        }
        articles.push_back(tmp);
        file.close();
    }
    readHistory();//��ȡ��ʷ��Ϣ
    return true;
}
bool forumSystem::save()
{//�洢��Ϣ
    fstream file;
    file.open(USER_PATH);
    //�ȴ��û���Ϣ
    for (auto i = users.begin(); i != users.end(); i++)
    {
        auto u = *i;
        if (i == --users.end())
        {
            string tmp = "*" + u.name + "*" + u.password + "*" + u.phone + "*" + u.mail + "*";
            file << tmp;
        }
        else
        {
            string tmp = "*" + u.name + "*" + u.password + "*" + u.phone + "*" + u.mail + "*";
            file << tmp << '\n';
        }
    }
    file.close();

    //�ٴ���ʷ��Ϣ
    file.open(HISTORY_PATH);

    for (auto i = history.begin(); i != history.end(); ++i)
    {
        auto& u = *i;
        string line;
        line += "*" + u.first + "*";//���û���
        for (auto& v : u.second)
            line += "*" + v + "*";//����û�����������
        if (i == --history.end())
        {
            file << line;
        }
        else
            file << line << "\n";

    }

    file.close();
    //�ٴ�������Ϣ

    unsigned int idcode = 10000;//���ӱ��
    for (auto& a : articles)
    {//��ÿһ������
        string road = CONTENT_PATH + string("/") + to_string(idcode) + ".txt";//�趨�洢·��
        ++idcode;
        //�����ļ�
        ofstream create(road);
        create.close();
        file.open(road);//�򿪶�Ӧ·���µ��ļ�
        size_t c = 30 - a.title.size();
        a.title.append(string(c, ' '));//����Ҫ�����30���ַ���
        file << a.title << "\n";//��title
        int i = 0;
        for (i = 0; i != a.contents.size() - 1; ++i)
        {//��¥��  #1#*���*cj2441962996*13545704716*2441962996@qq.com*#time#text#
            auto& l = a.contents[i];
            file << "#"<<l.floor <<"#" <<
                "*"<<l.publisher.name << "*" <<l.publisher.password << "*"<< l.publisher.phone << "*" << l.publisher.mail << "*"
                << "#" << l.time << "#" << l.text << "#" << "\n";
        }
        auto& l = a.contents[i];//��ֹ��ʽ����
        file << "#"<<l.floor << "#" <<
            "*" << l.publisher.name << "*" << l.publisher.password << "*" << l.publisher.phone << "*" << l.publisher.mail << "*"
            << "#" << l.time << "#" << l.text << "#";

        file.close();
    }

    return true;

}
bool forumSystem::exit()
{
    return save();
}
bool forumSystem::readHistory()
{//*���*title*title*
    fstream file;
    file.open(HISTORY_PATH);

    while (file.peek() != EOF)
    {
        string line;
        file >> line;
        StringList datas = spilt(line, "*");
        string username = datas[0];
        set<string> visited;
        for (int i = 1; i != datas.size(); ++i)
            visited.insert(datas[i]);
        history.insert({ username,visited });
    }
    file.close();
    return true;
}
/*****************************************************************************************************/

/*****************************************************************************************************/
bool article::Append(user u, const string& text)
{//content��ʽ:#floor#*user*#time#text#
    content tmp;
    tmp.floor = unsigned int(contents[contents.size() - 1].floor + 1);
    tmp.publisher = u;
    tmp.time = getTime();
    tmp.text = text;

    contents.push_back(tmp);
    return true;
}
void forumSystem::find()
{
    cout << "1.�ؼ��ֲ�ѯ\n";
    cout << "2.�û�����ѯ\n";
    cout << "3.����\n";
    cout << "�����Լ���:";
    int op = 0;
    cin >> op;
    switch (op)
    {
    case 1:
    {
        cout << "�ؼ��ֲ�ѯ" << endl;
        cout << "������ؼ���:";
        string key;
        cin >> key;
        map<article, vector<pair<int,string>>>result;//���� �� �ҵ���¥����Ϣ
        //�ִ�ƥ���㷨 
        for (auto& a : articles)
            for (auto& c : a.contents)
                if (hasKey(c.text, key))
                    result[a].push_back({ c.floor ,c.text });
        //��Ϊ��ѯ�Ľ�� ���� - ¥����Ϣ
        if (result.empty())
        {
            cout << "δ�ҵ�....." << endl;
            system("pause");
            system("cls");
            find();
            break;
        }
        else
        {
            cout << "�ҵ�" << result.size() << "������" << endl;
            system("pause");
            system("cls");

            for (auto& a : result)
            {
                cout << "----------------------------------------" << endl;
                cout << "���������ơ� �� " << a.first.title << endl;
                cout << "������ʱ�䡿 �� " << a.first.time  << endl;
                for (auto& x : a.second)
                {
                    cout << "----------------------------------------" << endl;
                    cout << "��¥�㡿 �� " << x.first << endl;
                    //x.secondҪת����\n
                    replace(x.second.begin(), x.second.end(), '_', '\n');
                    cout << "�����ݡ� �� " << endl << x.second << endl;
                    cout << "----------------------------------------" << endl;
                }
                cout << endl << endl;

            }
            cout << "----------------------------------------" << endl;
        }
        system("pause");
        system("cls");
        find();
        break;
    }
    case 2:
    {
        cout << "�û�����ѯ" << endl;
        cout << "�������û���:";
        string key;
        cin >> key;
        map<article, vector<pair<int, string>>>result;
        for (auto& a : articles)
        {//��ÿһ������
            for (auto& c : a.contents)
            {//��ÿһ��¥��
                if (c.publisher.name == key)
                {
                    result[a].push_back({ c.floor,c.text });
                }
            }
        }
        if (result.empty())
        {
            cout << "δ�ҵ�....." << endl;
            system("pause");
            system("cls");
            find();
        }
        else
        {
            cout << "�ҵ�" << result.size() << "������" << endl;
            system("pause");
            system("cls");

            for (auto& a : result)
            {
                cout << "----------------------------------------" << endl;
                cout << "���������ơ� �� " << a.first.title << endl;
                cout << "������ʱ�䡿 �� " << a.first.time << endl;
                for (auto& x : a.second)
                {
                    cout << "----------------------------------------" << endl;
                    cout << "��¥�㡿 �� " << x.first << endl;
                    //x.secondҪת����\n
                    replace(x.second.begin(), x.second.end(), '_', '\n');
                    cout << "�����ݡ� �� " << endl << x.second << endl;
                    cout << "----------------------------------------" << endl;
                }
                cout << endl << endl;

            }
            cout << "----------------------------------------" << endl;
        }
        system("pause");
        system("cls");
        find();
        break;
    }
    case 3:
        system("cls");
        return;
        break;
    default:
        cout << "��������";
        system("pause");
        system("cls");
        find();
        break;
    }
}
size_t forumSystem::index(const article& a)
{
    for (int i = 0; i != articles.size(); ++i)
    {
        if (articles[i].time == a.time&& articles[i].title == a.title)//ʱ����ͬ ��Ϊͬһƪ���� ���������ϵͳ�����
            return i;
    }
    return 0;
}
void forumSystem::showByTime()
{
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl;
    auto x = articles;
    sort(x.begin(), x.end(), [=](const article& r1, const article& r2) {return r1.time < r2.time; });
    for (auto& a : x)
    {

        string line;
        line += "       ";//������Ϣ
        if (history[me.name].find(a.title) != history[me.name].end())       //��ʷ���ʼ�¼
            line += "-   ";//״̬��Ϣ
        else
            line += "+   ";
        line += "          ";
        line += to_string(10000 + index(a));
        line += "          ";
        size_t c1 = 30 - a.title.size();
        line += a.title + string(c1, ' ');
        line += "            ";
        string num = to_string(a.contents.size());
        size_t c = 6 - num.size();
        line += num + string(c, ' ');
        line += "         ";
        string name = a.publisher.name;
        c = 12 - name.size();
        line += name + string(c, ' ');
        line += "           ";
        string tmp = a.contents[a.contents.size() - 1].publisher.name;
        c = 12 - tmp.size();
        line += tmp + string(c, ' ');
        line += "          ";
        line += a.contents[a.contents.size() - 1].time;
        line += "          ";
        cout << line << endl;

    }
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl << endl;


}
void forumSystem::showByNum() 
{
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl;

    auto x = articles;
    sort(x.begin(), x.end(), [=](const article& r1, const article& r2) {return r1.contents.size() < r2.contents.size(); });
    for (auto& a : x)
    {

        string line;
        line += "       ";//������Ϣ
        if (history[me.name].find(a.title) != history[me.name].end())       //��ʷ���ʼ�¼
            line += "-   ";//״̬��Ϣ
        else
            line += "+   ";
        line += "          ";
        line += to_string(10000 + index(a));
        line += "          ";
        size_t c1 = 30 - a.title.size();
        line += a.title + string(c1, ' ');
        line += "            ";
        string num = to_string(a.contents.size());
        size_t c = 6 - num.size();
        line += num + string(c, ' ');
        line += "         ";
        string name = a.publisher.name;
        c = 12 - name.size();
        line += name + string(c, ' ');
        line += "           ";
        string tmp = a.contents[a.contents.size() - 1].publisher.name;
        c = 12 - tmp.size();
        line += tmp + string(c, ' ');
        line += "          ";
        line += a.contents[a.contents.size() - 1].time;
        line += "          ";
        cout << line << endl;

    }
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl << endl;


}
void forumSystem::showByTitle()
{
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl;
    auto x = articles;
    sort(x.begin(), x.end(), [=](const article& r1, const article& r2) {return r1.title < r2.title; });
    for (auto& a : x)
    {

        string line;
        line += "       ";//������Ϣ
        if (history[me.name].find(a.title) != history[me.name].end())       //��ʷ���ʼ�¼
            line += "-   ";//״̬��Ϣ
        else
            line += "+   ";
        line += "          ";
        line += to_string(10000 + index(a));
        line += "          ";
        size_t c1 = 30 - a.title.size();
        line += a.title + string(c1, ' ');
        line += "            ";
        string num = to_string(a.contents.size());
        size_t c = 6 - num.size();
        line += num + string(c, ' ');
        line += "         ";
        string name = a.publisher.name;
        c = 12 - name.size();
        line += name + string(c, ' ');
        line += "           ";
        string tmp = a.contents[a.contents.size() - 1].publisher.name;
        c = 12 - tmp.size();
        line += tmp + string(c, ' ');
        line += "          ";
        line += a.contents[a.contents.size() - 1].time;
        line += "          ";
        cout << line << endl;

    }
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl << endl;


}
void forumSystem::showByCode()
{
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl;
    //Ĭ�ϰ��ձ������  �� ��ȡ˳��
    int count = 10000;
    for (auto& a : articles)
    {
        string line;
        line += "       ";//������Ϣ
        if (history[me.name].find(a.title) != history[me.name].end())       //��ʷ���ʼ�¼
            line += "-   ";//״̬��Ϣ
        else
            line += "+   ";
        line += "          ";
        line += to_string(count++);
        line += "          ";
        size_t c1 = 30 - a.title.size();
        line += a.title + string(c1, ' ');
        line += "            ";
        string num = to_string(a.contents.size());
        size_t c = 6 - num.size();
        line += num + string(c, ' ');
        line += "         ";
        string name = a.publisher.name;
        c = 12 - name.size();
        line += name + string(c, ' ');
        line += "           ";
        string tmp = a.contents[a.contents.size() - 1].publisher.name;
        c = 12 - tmp.size();
        line += tmp + string(c, ' ');
        line += "          ";
        line += a.contents[a.contents.size() - 1].time;
        line += "          ";
        cout << line << endl;

    }
    cout << "=======" << "====" << "==========" << "��� " << "==========" << "����==========================" << "===========" << "�ظ���" << "==========" << "����========" << "==========" << "���ظ�====" << "==========" << "�ظ�ʱ��==============" << "==========" << endl << endl;
}
void forumSystem::mainWindow(size_t o)
{//��̳��ҳ��
    cout << "�û�: " << me.name << endl;

    //�������������Ϣ
    //��������  ��articles �����ݽ������򼴿�
    switch (o)
    {
    case 1:showByCode(); break;
    case 2:showByTime(); break;
    case 3:showByNum(); break;
    case 4:showByTitle(); break;
    default:
        break;
    }


    cout << "1.�������" << endl; 
    cout << "2.��������" << endl;
    cout << "3.��������" << endl;
    cout << "4.��������" << endl;
    cout << "5.�˳�ϵͳ" << endl;
    cout << "�����Լ���: ";
    int control = 0;
    cin >> control;

    switch (control)
    {
    case 1:
    {
        cout << "�������ӱ���Լ���: ";
        size_t index = 0;
        cin >> index;
        index -= 10000;

        size_t max = articles.size();
        if (0 <= index && index < max)
        {//���ڴ�����
            cout << "�ҵ�����" << endl;
            system("pause");
            system("cls");
            articleShow(int(index));
            mainWindow();
        }
        else
        {
            cout << "�����ڴ˱������" << endl;
            system("pause");
            system("cls");
            mainWindow();
        }
        break;
    }
    case 2:
    {
        publish();
        mainWindow();
        break;
    }
    case 3:
    {
        system("cls");
        find();
        mainWindow();
        break;
    }
    case 4:
    {        
        cout << "1.�������" << endl;
        cout << "2.ʱ������" << endl;
        cout << "3.�ظ�����" << endl;
        cout << "4.��������" << endl;
        cout << "��������������:";
        size_t op;
        cin >> op;
        system("cls");
        switch (op)
        {
        case 1:return mainWindow(1); break;
        case 2:return mainWindow(2); break;
        case 3:return mainWindow(3); break;
        case 4:return mainWindow(4); break;
        default:
            cout << "��������" << endl;
            system("pause");
            system("cls");
            return mainWindow();
            break;
        }
        break;
    }
    case 5:
    {
        exit();
        return;
        break;
    }
    default:
    {
        cout << "ERROR" << endl;
        system("pause");
        system("cls");
        mainWindow();
        break;
    }
    }
}
void forumSystem::articleShow(const int& index)
{//չʾ����
    
    auto& now = articles[index];
    cout << now.title << endl;

    auto& visited = history[me.name];//��ȡ�Ѿ����ʵ�
    visited.insert(now.title);

    for (auto& c : now.contents)
    {//���ÿһ¥��
        cout << "----------------------------------------------------------------------" << endl;
        cout << "��¥�㡿��" << c.floor << "    " << c.publisher.name << "   " << "����ʱ�䣺" << c.time << endl;
        StringList tmp = spilt(c.text, "_");//���ֻ���
        for (auto& x : tmp)
            cout << x << endl;
        cout << "----------------------------------------------------------------------" << endl;
    }

    cout << endl << endl;
    cout << "1.����" << endl;
    cout << "2.ɾ��" << endl;
    cout << "3.����" << endl;
    cout << "�����Լ���";
    size_t control = 0;
    cin >> control;

    switch (control)
    {
    case 1:
    {
        cout << "�������������(��\"_\"���滻��)(Enter�����������)" << endl;
        string text;
        cin >> text;
        now.Append(me, text);
        cout << "�����ɹ�" << endl;
        system("pause");
        system("cls");
        articleShow(index);
        break;
    }
    case 2:
    {
        cout << "����Ҫɾ����¥��(ֻ��ɾ���Լ���¥��Ŷ~)" << endl;
        unsigned int i = 0;
        cin >> i;
        for (auto b = now.contents.begin(); b != now.contents.end(); ++b)
        {
            if (b->floor == i && b->publisher == me)
            {
                cout << "�ҵ�¥��" << endl;
                now.contents.erase(b);
                cout << "��ɾ��" << endl;
                system("pause");
                system("cls");
                return articleShow(index);
            }
        }
        cout << "δ�ҵ�¥��" << endl;
        system("pause");
        system("cls");
        articleShow(index);
        break;
    }
    case 3:
    {
        system("cls");
        return;
        break;
    }
    default:
    {
        cout << "��������" << endl;
        system("pause");
        system("cls");
        return articleShow(index);
        break;
    }
    }
}
bool forumSystem::login()
{//����̨Ӧ������Ϣ��Ӧ��ʹ��   Ҳ������
    //��¼
    cout << "��ӭ" << endl;
    cout << "1.��¼" << endl;
    cout << "2.ע�����û�" << endl;
    int control = 0;
    cin >> control;
    switch (control)
    {
    case 1:
    {
        cout << "�����û���: ";
        string name;
        cin >> name;
        cout << "��������: ";
        string password;
        cin >> password;

        if (users.find(user(name)) != users.end())
        {//���ڸ��û�
            if ((*users.find(user(name))).password == password)
            {//������ȷô
                me = *users.find(user(name));//���õ�ǰ��¼�û�
                cout << "��¼�ɹ�";
                system("pause");
                system("cls");
                mainWindow();//ת��������
            }
            else
            {
                cout << "������� ������";
                system("pause");
                system("cls");
                login();
            }
        }
        else
        {//�����ڸ��û�
            cout << "�����ڸ��û�  ����ע��";
            system("pause");
            system("cls");
            login();
        }
        break;
    }
    case 2:
    {
        if (regedit())
        {//ע��ɹ�
            cout << "ע��ɹ�";
            system("pause");
            system("cls");
            login();
        }
        else
        {//ע��ʧ��
            cout << "ע��ʧ��";
            system("pause");
            system("cls");
            login();
        }
        break;
    }
    default:
    {
        cout << "ERROR";
        system("pause");
        system("cls");
        login();
        break;
    }
    }
    return true;
}
void forumSystem::publish()
{
    system("cls");
    cout << "��������" << endl;
    cout << "--------" << endl;
    cout << "�������ӱ���: ";
    string title;
    cin >> title;
    cout << "�����������ݣ�_����Enter,��Ҫʹ�ÿո�: ";
    string text;
    cin >> text;


    content tmp(1, me, getTime(), text);
    article x;
    x.contents.push_back(tmp);
    x.publisher = me;
    x.time = getTime();
    x.title = title;

    this->articles.push_back(x);
    cout << "�����ɹ�";
    system("pause");
    system("cls");
    return;
}
bool forumSystem::regedit()
{//ע��
    cout << "ע�����û�: ";
    cout << "�����û���: ";
    string name;
    cin >> name;
    //�û����ظ����
    if (users.find(user(name)) != users.end())
    {//���ڸ��û�
        cout << "�Ѿ����ڸ��û���" << endl;
        return false;
    }
    //������ȷ�Լ��
    cout << "��������: ";
    string password;
    cin >> password;
    if (password.size() < 6)
    {
        cout << "���볤�Ȳ�������6λ" << endl;
        return false;
    }
    bool hasOther = false;//��ʶλ
    for (auto& c : password)
        if (isdigit(c))
            continue;
        else
        {
            hasOther = true;
            break;
        }
    if (!hasOther)
    {
        cout << "���벻��ֻ������" << endl;
        return false;
    }

    cout << "����绰����: ";
    string phone;
    cin >> phone;
    //�绰������ȷ�Լ��
    if (phone.size() != 11)
    {
        cout << "�绰�����ʽ����" << endl;
        return false;
    }
    for(auto&c:phone)
        if (!isdigit(c))
        {
            cout << "�绰����ֻ�ܰ�������" << endl;
            return false;
        }
    cout << "�����������";
    string mail;
    cin >> mail;
    //����������ȷ�Լ��
    int AtCount = 0;
    if (mail.empty())
    {
        cout << "mailΪ��" << endl;
        return false;
    }
    for (int i = 0; i != mail.size(); ++i)
    {
        if (mail[i] == '@')
            ++AtCount;
    }
    if (AtCount == 1 && mail[0] != '@' && mail[mail.size() - 1] != '@')
    {//��1��@�Ҳ��ڿ�ͷ���β
        //ȫ����ȷ
        user u(name, password, phone, mail);//�������û�
        users.insert(u);
        return true;
    }
    else
    {
        cout << "���������ʽ����" << endl;
        return false;
    }


    

}
/*****************************************************************************************************/




/*****************************************************************************************************/
vector<string> spilt(string str, const string& pattern)
{//ʵ������QString���ַ����ָ��
    string::size_type pos;
    vector<string>result;
    str += pattern;//��һ��������־
    for (size_t i = 0; i != str.size(); ++i)
    {
        pos = str.find(pattern, i);//��locate i ��ʼ ����pattern
        if (pos != string::npos)
        {//�ҵ�
            auto tmp = str.substr(i, pos - i);
            if (!tmp.empty())
                result.push_back(tmp);//��Ӧ��Χ�����ַ���
            i = pos + pattern.size() - 1;//�ƶ�i�����pattern��ĩβ
        }
    }
    return result;
}
void getFiles(string path, vector<string>& files)
{
    intptr_t   hFile = 0;//�ļ�������������������
    struct _finddata_t fileinfo;//�ļ���Ϣ
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
        //������ҵ���һ���ļ�
    {
        do
        {
            if ((fileinfo.attrib & _A_SUBDIR))//������ļ���
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("/").append(fileinfo.name), files);
            }
            else//������ļ�
            {
                files.push_back(p.assign(path).append("/").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);	//��Ѱ�ҵ������ļ�

        _findclose(hFile);	//�������ң��رվ��
    }
}
user toUser(const string& str) 
{//��ʽ��Ϣ  ���*cj2441962996*13545704716*2441962996@qq.com  ANIS����
    StringList tmp = spilt(str, "*");
    return user(tmp[0], tmp[1], tmp[2], tmp[3]);
}
content toContent(const string& str)
{//��ʽ��Ϣ #floor#user#time#text#
    StringList text = spilt(str, "#");
    user u(toUser(text[1]));
    return content(stoi(text[0]), u, text[2], text[3]);
}
string getTime()
{
    struct tm t;
    time_t now;
    time(&now);//��ȡʱ��
    localtime_s(&t, &now);
    char buffer[1024];
    sprintf_s(buffer, "%d-%d-%d__%d:%d:%d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

    return string(buffer);;
}
bool hasKey(const string& str, const string& key)
{//�ж�str���Ƿ���key
    //KMP�㷨
    int tLen = str.size();
    int pLen = key.size();
    int i = 0, j = 0;

    while (i < tLen && j < pLen)
    {
        if (str[i] == key[j])
        {
            ++i;
            ++j;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j == pLen)
        return true;
    else
        return false;
}
/*****************************************************************************************************/