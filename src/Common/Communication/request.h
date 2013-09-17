#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <QObject>

class Request
{
public:
    Request()
    {
        prefix_[0] = 'C';
        prefix_[1] = 'Y';
        prefix_[2] = 'T';

        cmd_id_ = 0;
        request_length_ = 0;
        request_content_ = NULL;
        reply_length_ = 0;
        reply_content_ = NULL;

        target_obj_ = NULL;
    }

    ~Request()
    {
        if (request_content_ != NULL)
        {
            delete [] request_content_;
            request_content_ = NULL;
        }
        if (reply_content_ != NULL)
        {
            delete [] reply_content_;
            reply_content_ = NULL;
        }
    }

    char prefix_[3];
    char cmd_id_;
    char *request_content_;
    char *reply_content_;
    unsigned int request_length_;
    unsigned int reply_length_;

    QObject *target_obj_;
    std::string target_slot_;
};

class ReplyHead
{
public:
    ReplyHead()
    {
        prefix_[0] = 'C';
        prefix_[1] = 'Y';
        prefix_[2] = 'T';

        cmd_id_ = '0';

//        suffix_[0] = 'E';
//        suffix_[1] = 'N';
//        suffix_[2] = 'D';
    }

    ~ReplyHead() { }

    char prefix_[3];
    char cmd_id_;
//    char suffix_[3];
};

class ReplyEnd
{
public:
    ReplyEnd()
    {
        suffix_[0] = 'E';
        suffix_[1] = 'N';
        suffix_[2] = 'D';
    }
    ~ReplyEnd() {}
    char suffix_[3];
};

#endif // REQUEST_H
