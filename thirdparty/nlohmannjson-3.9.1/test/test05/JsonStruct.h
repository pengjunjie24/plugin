
#pragma once
#include <json.hpp>

#include <stdint.h>
#include <string>

using json = nlohmann::json;

//根据json文件格式定义结构体

//VideoInfo对应json文件中output
struct VideoInfo
{
    int32_t width;
    int32_t height;
    int32_t frameRate;
    int32_t crf;
};

//json对象转C++结构体函数
void from_json(const json& j, VideoInfo& v)
{
    j.at("width").get_to(v.width);
    j.at("height").get_to(v.height);
    j.at("frameRate").get_to(v.frameRate);
    j.at("crf").get_to(v.crf);
}

//C++结构体转json对象函数
void to_json(json& j, const VideoInfo& v)
{
    j = json{ {"width", v.width}, {"height", v.height},
        {"frameRate", v.frameRate}, {"crf", v.crf} };
}

//PieceInfo对应json文件中pieces
struct PieceInfo
{
    std::string  pathname;
    int32_t startTime;
    int32_t endTime;
};

void from_json(const json& j, PieceInfo& p)
{
    j.at("file").get_to(p.pathname);
    j.at("startTime").get_to(p.startTime);
    j.at("endTime").get_to(p.endTime);
}

void to_json(json& j, const PieceInfo& p)
{
    j = json{ {"file", p.pathname}, {"startTime", p.startTime},
        {"endTime", p.endTime} };
}

//TrackInfo对应json文件中tracks
struct TrackInfo
{
    std::string name;
    PieceInfo pieces[10];
    int32_t size;
};

void from_json(const json& j, TrackInfo &t)
{
    j.at("name").get_to(t.name);
    for (size_t i = 0; i < j["pieces"].size(); i++)
    {
        t.pieces[i] = j["pieces"][i];
    }
    t.size = j["pieces"].size();
}

void to_json(json& j, const TrackInfo& t)
{
    j["name"] = t.name;
    for (size_t i = 0; i < (size_t)t.size ; ++i)
    {
        json videoJson = t.pieces[i];
        j["pieces"][i] = videoJson;
    }
    j["size"] = t.size;
}
