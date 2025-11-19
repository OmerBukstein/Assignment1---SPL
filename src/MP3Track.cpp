#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    //prints main message
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    //prints tags check
    if (has_id3_tags == true){
        std::cout <<"  -> Processing ID3 metadata (artist info, album art, etc.)"<< std::endl;
    }
    else{
        std::cout <<"  -> No ID3 tags found."<<std::endl;
    }
    //prints decoding and ending process messages
    std::cout <<"  -> Decoding MP3 frames..."<<std::endl;
    std::cout <<"  -> Load complete."<<std::endl;
}

void MP3Track::analyze_beatgrid() {
    std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    //Calculate estimated beats
    double beats = (duration_seconds / 60.0) * bpm;
    //Calculate compression precision factor based on bitrate
    double precision_factor = bitrate /320.0;
    std::cout <<"  -> Estimated beats:  " <<beats<< "  -> Compression precision factor:  "<<precision_factor<< std::endl;
}

double MP3Track::get_quality_score() const {
    double base_score = (bitrate/320.0) * 100.0;
    if (has_id3_tags == true){
        base_score = base_score + 5.0;
    }
    if (bitrate<128){
        base_score = base_score-10.0;
    }
    if (base_score<0){
        return 0.0;
    }
    else if (base_score>100){
        return 100.0;
    }
    return base_score; 
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    return PointerWrapper<AudioTrack>(new MP3Track(*this)); // Replace with your implementation
}