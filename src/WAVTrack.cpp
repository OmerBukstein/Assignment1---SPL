#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    //prints loading message
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
            << "\" at " << sample_rate << "Hz/" << bit_depth <<"bit (uncompressed)...\n";
    long long size = duration_seconds * sample_rate * (bit_depth / 8) *2;
    std::cout <<"  -> Estimated file size: " << size << " bytes" << std::endl; 
    std::cout <<"  -> Fast loading due to uncompressed format." << std::endl; 
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    int beats = (duration_seconds / 60.0) * bpm; //is beats should be double or int?
    std::cout <<"  -> Estimated beats:"<< beats <<"  -> Precision factor: 1 (uncompressed audio)" << std::endl; 
}

double WAVTrack::get_quality_score() const {
    double base_score = 70.0;
    if (sample_rate >= 44100){ //bonus of CD quality
        base_score = base_score + 10.0;
    }
    if (sample_rate >= 96000){ //bonus of high-res audio
        base_score = base_score + 5.0;
    }
    if (bit_depth >=16){ //bonus of CD quality
        base_score = base_score + 10.0;
    }
        if (bit_depth >=24){ //bonus of professional quality
        base_score = base_score + 5.0;
    }
    if (base_score>100){
        return 100.0;
    }
    return base_score; 
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(new WAVTrack(*this)); // Replace with your implementation
}