#include "forumSystem.h"

/*****************************************************************************************************/
bool forumSystem::read()
{//读取论坛信息
    fstream file;
    file.open(USER_PATH);//先读用户信息 到 users
    
    while (file.peek() != EOF)
    {
        string u;
        file >> u;
        users.insert(toUser(u));
    }
    file.close();

    //读取帖子信息
    //先读取目录下的所有帖子路径
    StringList roads;
    getFiles(CONTENT_PATH, roads);

        //帖子格式
        //[矩形相交]问题讨论帖 
        //*floor* name* time* text*

    for (auto& road : roads)
    {//对每一个帖子

        file.open(road);

        article tmp;
        //第一行是标题
        file >> tmp.title;

        //第二行是一楼（发布者）  帖子信息读取
        string str;
        file >> str;
        content c(toContent(str));
        tmp.publisher = c.publisher;
        tmp.time = c.time;
        tmp.contents.push_back(c);

        while (file.peek() != EOF)
        {//后续楼层读取
            file >> str;
            tmp.contents.push_back(toContent(str));
        }
        articles.push_back(tmp);
        file.close();
    }
    readHistory();//读取历史信息
    return true;
}
bool forumSystem::save()
{//存储信息
    fstream file;
    file.open(USER_PATH);
    //先存用户信息
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

    //再存历史信息
    file.open(HISTORY_PATH);

    for (auto i = history.begin(); i != history.end(); ++i)
    {
        auto& u = *i;
        string line;
        line += "*" + u.first + "*";//存用户名
        for (auto& v : u.second)
            line += "*" + v + "*";//存该用户看过的帖子
        if (i == --history.end())
        {
            file << line;
        }
        else
            file << line << "\n";

    }

    file.close();
    //再存帖子信息

    unsigned int idcode = 10000;//帖子编号
    for (auto& a : articles)
    {//对每一个帖子
        string road = CONTENT_PATH + string("/") + to_string(idcode) + ".txt";//设定存储路径
        ++idcode;
        //创建文件
        ofstream create(road);
        create.close();
        file.open(road);//打开对应路径下的文件
        size_t c = 30 - a.title.size();
        a.title.append(string(c, ' '));//标题要构造成30个字符的
        file << a.title << "\n";//存title
        int i = 0;
        for (i = 0; i != a.contents.size() - 1; ++i)
        {//存楼层  #1#*卷卷*cj2441962996*13545704716*2441962996@qq.com*#time#text#
            auto& l = a.contents[i];
            file << "#"<<l.floor <<"#" <<
                "*"<<l.publisher.name << "*" <<l.publisher.password << "*"<< l.publisher.phone << "*" << l.publisher.mail << "*"
                << "#" << l.time << "#" << l.text << "#" << "\n";
        }
        auto& l = a.contents[i];//防止格式错误
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
{//*卷卷*title*title*
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
{//content格式:#floor#*user*#time#text#
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
    cout << "1.关键字查询\n";
    cout << "2.用户名查询\n";
    cout << "3.返回\n";
    cout << "输入以继续:";
    int op = 0;
    cin >> op;
    switch (op)
    {
    case 1:
    {
        cout << "关键字查询" << endl;
        cout << "请输入关键字:";
        string key;
        cin >> key;
        map<article, vector<pair<int,string>>>result;//帖子 和 找到的楼层信息
        //字串匹配算法 
        for (auto& a : articles)
            for (auto& c : a.contents)
                if (hasKey(c.text, key))
                    result[a].push_back({ c.floor ,c.text });
        //即为查询的结果 帖子 - 楼层信息
        if (result.empty())
        {
            cout << "未找到....." << endl;
            system("pause");
            system("cls");
            find();
            break;
        }
        else
        {
            cout << "找到" << result.size() << "个帖子" << endl;
            system("pause");
            system("cls");

            for (auto& a : result)
            {
                cout << "----------------------------------------" << endl;
                cout << "【帖子名称】 ： " << a.first.title << endl;
                cout << "【发表时间】 ： " << a.first.time  << endl;
                for (auto& x : a.second)
                {
                    cout << "----------------------------------------" << endl;
                    cout << "【楼层】 ： " << x.first << endl;
                    //x.second要转化成\n
                    replace(x.second.begin(), x.second.end(), '_', '\n');
                    cout << "【内容】 ： " << endl << x.second << endl;
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
        cout << "用户名查询" << endl;
        cout << "请输入用户名:";
        string key;
        cin >> key;
        map<article, vector<pair<int, string>>>result;
        for (auto& a : articles)
        {//对每一个帖子
            for (auto& c : a.contents)
            {//对每一个楼层
                if (c.publisher.name == key)
                {
                    result[a].push_back({ c.floor,c.text });
                }
            }
        }
        if (result.empty())
        {
            cout << "未找到....." << endl;
            system("pause");
            system("cls");
            find();
        }
        else
        {
            cout << "找到" << result.size() << "个帖子" << endl;
            system("pause");
            system("cls");

            for (auto& a : result)
            {
                cout << "----------------------------------------" << endl;
                cout << "【帖子名称】 ： " << a.first.title << endl;
                cout << "【发表时间】 ： " << a.first.time << endl;
                for (auto& x : a.second)
                {
                    cout << "----------------------------------------" << endl;
                    cout << "【楼层】 ： " << x.first << endl;
                    //x.second要转化成\n
                    replace(x.second.begin(), x.second.end(), '_', '\n');
                    cout << "【内容】 ： " << endl << x.second << endl;
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
        cout << "输入有误";
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
        if (articles[i].time == a.time&& articles[i].title == a.title)//时间相同 即为同一篇帖子 至少在这个系统中如此
            return i;
    }
    return 0;
}
void forumSystem::showByTime()
{
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl;
    auto x = articles;
    sort(x.begin(), x.end(), [=](const article& r1, const article& r2) {return r1.time < r2.time; });
    for (auto& a : x)
    {

        string line;
        line += "       ";//对齐信息
        if (history[me.name].find(a.title) != history[me.name].end())       //历史访问记录
            line += "-   ";//状态信息
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
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl << endl;


}
void forumSystem::showByNum() 
{
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl;

    auto x = articles;
    sort(x.begin(), x.end(), [=](const article& r1, const article& r2) {return r1.contents.size() < r2.contents.size(); });
    for (auto& a : x)
    {

        string line;
        line += "       ";//对齐信息
        if (history[me.name].find(a.title) != history[me.name].end())       //历史访问记录
            line += "-   ";//状态信息
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
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl << endl;


}
void forumSystem::showByTitle()
{
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl;
    auto x = articles;
    sort(x.begin(), x.end(), [=](const article& r1, const article& r2) {return r1.title < r2.title; });
    for (auto& a : x)
    {

        string line;
        line += "       ";//对齐信息
        if (history[me.name].find(a.title) != history[me.name].end())       //历史访问记录
            line += "-   ";//状态信息
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
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl << endl;


}
void forumSystem::showByCode()
{
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl;
    //默认按照编号排序  即 读取顺序
    int count = 10000;
    for (auto& a : articles)
    {
        string line;
        line += "       ";//对齐信息
        if (history[me.name].find(a.title) != history[me.name].end())       //历史访问记录
            line += "-   ";//状态信息
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
    cout << "=======" << "====" << "==========" << "编号 " << "==========" << "主题==========================" << "===========" << "回复数" << "==========" << "作者========" << "==========" << "最后回复====" << "==========" << "回复时间==============" << "==========" << endl << endl;
}
void forumSystem::mainWindow(size_t o)
{//论坛主页面
    cout << "用户: " << me.name << endl;

    //输出所有帖子信息
    //四种排序  对articles 的内容进行排序即可
    switch (o)
    {
    case 1:showByCode(); break;
    case 2:showByTime(); break;
    case 3:showByNum(); break;
    case 4:showByTitle(); break;
    default:
        break;
    }


    cout << "1.浏览帖子" << endl; 
    cout << "2.发布帖子" << endl;
    cout << "3.查找帖子" << endl;
    cout << "4.重新排序" << endl;
    cout << "5.退出系统" << endl;
    cout << "输入以继续: ";
    int control = 0;
    cin >> control;

    switch (control)
    {
    case 1:
    {
        cout << "输入帖子编号以继续: ";
        size_t index = 0;
        cin >> index;
        index -= 10000;

        size_t max = articles.size();
        if (0 <= index && index < max)
        {//存在此帖子
            cout << "找到此贴" << endl;
            system("pause");
            system("cls");
            articleShow(int(index));
            mainWindow();
        }
        else
        {
            cout << "不存在此编号帖子" << endl;
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
        cout << "1.编号排序" << endl;
        cout << "2.时间排序" << endl;
        cout << "3.回复排序" << endl;
        cout << "4.标题排序" << endl;
        cout << "输入以重新排序:";
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
            cout << "错误输入" << endl;
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
{//展示帖子
    
    auto& now = articles[index];
    cout << now.title << endl;

    auto& visited = history[me.name];//获取已经访问的
    visited.insert(now.title);

    for (auto& c : now.contents)
    {//输出每一楼层
        cout << "----------------------------------------------------------------------" << endl;
        cout << "【楼层】：" << c.floor << "    " << c.publisher.name << "   " << "发布时间：" << c.time << endl;
        StringList tmp = spilt(c.text, "_");//划分换行
        for (auto& x : tmp)
            cout << x << endl;
        cout << "----------------------------------------------------------------------" << endl;
    }

    cout << endl << endl;
    cout << "1.回帖" << endl;
    cout << "2.删帖" << endl;
    cout << "3.返回" << endl;
    cout << "输入以继续";
    size_t control = 0;
    cin >> control;

    switch (control)
    {
    case 1:
    {
        cout << "请输入回帖内容(用\"_\"代替换行)(Enter代表输入结束)" << endl;
        string text;
        cin >> text;
        now.Append(me, text);
        cout << "回帖成功" << endl;
        system("pause");
        system("cls");
        articleShow(index);
        break;
    }
    case 2:
    {
        cout << "输入要删除的楼层(只能删除自己的楼层哦~)" << endl;
        unsigned int i = 0;
        cin >> i;
        for (auto b = now.contents.begin(); b != now.contents.end(); ++b)
        {
            if (b->floor == i && b->publisher == me)
            {
                cout << "找到楼层" << endl;
                now.contents.erase(b);
                cout << "已删除" << endl;
                system("pause");
                system("cls");
                return articleShow(index);
            }
        }
        cout << "未找到楼层" << endl;
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
        cout << "输入有误" << endl;
        system("pause");
        system("cls");
        return articleShow(index);
        break;
    }
    }
}
bool forumSystem::login()
{//控制台应用做消息响应很痛苦   也不美观
    //登录
    cout << "欢迎" << endl;
    cout << "1.登录" << endl;
    cout << "2.注册新用户" << endl;
    int control = 0;
    cin >> control;
    switch (control)
    {
    case 1:
    {
        cout << "输入用户名: ";
        string name;
        cin >> name;
        cout << "输入密码: ";
        string password;
        cin >> password;

        if (users.find(user(name)) != users.end())
        {//存在该用户
            if ((*users.find(user(name))).password == password)
            {//密码正确么
                me = *users.find(user(name));//设置当前登录用户
                cout << "登录成功";
                system("pause");
                system("cls");
                mainWindow();//转到主界面
            }
            else
            {
                cout << "密码错误 请重试";
                system("pause");
                system("cls");
                login();
            }
        }
        else
        {//不存在该用户
            cout << "不存在该用户  请先注册";
            system("pause");
            system("cls");
            login();
        }
        break;
    }
    case 2:
    {
        if (regedit())
        {//注册成功
            cout << "注册成功";
            system("pause");
            system("cls");
            login();
        }
        else
        {//注册失败
            cout << "注册失败";
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
    cout << "发布帖子" << endl;
    cout << "--------" << endl;
    cout << "输入帖子标题: ";
    string title;
    cin >> title;
    cout << "输入主题内容（_代替Enter,不要使用空格）: ";
    string text;
    cin >> text;


    content tmp(1, me, getTime(), text);
    article x;
    x.contents.push_back(tmp);
    x.publisher = me;
    x.time = getTime();
    x.title = title;

    this->articles.push_back(x);
    cout << "发布成功";
    system("pause");
    system("cls");
    return;
}
bool forumSystem::regedit()
{//注册
    cout << "注册新用户: ";
    cout << "输入用户名: ";
    string name;
    cin >> name;
    //用户名重复检测
    if (users.find(user(name)) != users.end())
    {//存在该用户
        cout << "已经存在该用户名" << endl;
        return false;
    }
    //密码正确性检测
    cout << "输入密码: ";
    string password;
    cin >> password;
    if (password.size() < 6)
    {
        cout << "密码长度不能少于6位" << endl;
        return false;
    }
    bool hasOther = false;//标识位
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
        cout << "密码不能只有数字" << endl;
        return false;
    }

    cout << "输入电话号码: ";
    string phone;
    cin >> phone;
    //电话号码正确性检测
    if (phone.size() != 11)
    {
        cout << "电话号码格式有误" << endl;
        return false;
    }
    for(auto&c:phone)
        if (!isdigit(c))
        {
            cout << "电话号码只能包含数字" << endl;
            return false;
        }
    cout << "输入电子邮箱";
    string mail;
    cin >> mail;
    //电子邮箱正确性检测
    int AtCount = 0;
    if (mail.empty())
    {
        cout << "mail为空" << endl;
        return false;
    }
    for (int i = 0; i != mail.size(); ++i)
    {
        if (mail[i] == '@')
            ++AtCount;
    }
    if (AtCount == 1 && mail[0] != '@' && mail[mail.size() - 1] != '@')
    {//有1个@且不在开头与结尾
        //全部正确
        user u(name, password, phone, mail);//构建新用户
        users.insert(u);
        return true;
    }
    else
    {
        cout << "电子邮箱格式错误" << endl;
        return false;
    }


    

}
/*****************************************************************************************************/




/*****************************************************************************************************/
vector<string> spilt(string str, const string& pattern)
{//实现类似QString的字符串分割函数
    string::size_type pos;
    vector<string>result;
    str += pattern;//给一个结束标志
    for (size_t i = 0; i != str.size(); ++i)
    {
        pos = str.find(pattern, i);//从locate i 开始 查找pattern
        if (pos != string::npos)
        {//找到
            auto tmp = str.substr(i, pos - i);
            if (!tmp.empty())
                result.push_back(tmp);//对应范围生成字符串
            i = pos + pattern.size() - 1;//移动i到这个pattern的末尾
        }
    }
    return result;
}
void getFiles(string path, vector<string>& files)
{
    intptr_t   hFile = 0;//文件句柄，过会儿用来查找
    struct _finddata_t fileinfo;//文件信息
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
        //如果查找到第一个文件
    {
        do
        {
            if ((fileinfo.attrib & _A_SUBDIR))//如果是文件夹
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("/").append(fileinfo.name), files);
            }
            else//如果是文件
            {
                files.push_back(p.assign(path).append("/").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);	//能寻找到其他文件

        _findclose(hFile);	//结束查找，关闭句柄
    }
}
user toUser(const string& str) 
{//格式信息  卷卷*cj2441962996*13545704716*2441962996@qq.com  ANIS编码
    StringList tmp = spilt(str, "*");
    return user(tmp[0], tmp[1], tmp[2], tmp[3]);
}
content toContent(const string& str)
{//格式信息 #floor#user#time#text#
    StringList text = spilt(str, "#");
    user u(toUser(text[1]));
    return content(stoi(text[0]), u, text[2], text[3]);
}
string getTime()
{
    struct tm t;
    time_t now;
    time(&now);//获取时间
    localtime_s(&t, &now);
    char buffer[1024];
    sprintf_s(buffer, "%d-%d-%d__%d:%d:%d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

    return string(buffer);;
}
bool hasKey(const string& str, const string& key)
{//判断str中是否含有key
    //KMP算法
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