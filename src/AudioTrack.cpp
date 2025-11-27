#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), waveform_data(nullptr),
      waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {
    // TODO: Implement the destructor
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    //delete waveform_data;
    #endif
    delete [] waveform_data; //waveform_data is an array, we need to dealocate all the memory in it
}

AudioTrack::AudioTrack(const AudioTrack& other): title(other.title), artists(other.artists), duration_seconds(other.duration_seconds), 
bpm(other.bpm), waveform_data(nullptr), waveform_size(other.waveform_size) //notice that every field is mentioned and by the order in the header
{
    // TODO: Implement the copy constructor
    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif
    if (waveform_size>0){ //if there id memory to copy - initialize in heap 
        waveform_data = new double[waveform_size];
        other.get_waveform_copy(waveform_data, waveform_size); // other is a const ref, so use get_waveform_copy must be const  
    }
    
}

AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    // TODO: Implement the copy assignment operator
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif
    if (this!=&other){
        //copy the fields of other to this
        title = other.title;
        artists = other.artists;
        duration_seconds = other.duration_seconds;
        bpm = other.bpm; 
        //dealocate waveform_data memory 
        delete [] waveform_data;
        waveform_data = nullptr; //initialize waveform_data  
        waveform_size = other.waveform_size;
        if (waveform_size>0){ //if we need to add data to waveform_data, do deep copy
            waveform_data = new double[waveform_size];
            other.get_waveform_copy(waveform_data, waveform_size);
        }   
    }
    return *this;
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept : title(std::move(other.title)), artists(std::move(other.artists)), duration_seconds(other.duration_seconds), 
bpm(other.bpm), waveform_data(other.waveform_data), waveform_size(other.waveform_size) {
    // TODO: Implement the move constructor
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    other.waveform_data = nullptr;
    other.waveform_size = 0;
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    // TODO: Implement the move assignment operator

    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif
    if (this != &other){
        //stealing the fields
        title = std::move(other.title);
        artists = std::move(other.artists);
        duration_seconds = other.duration_seconds;
        bpm = other.bpm;
        //dealocate "old" array from heap memory
        delete [] waveform_data;            
        //stealing the array from other
        waveform_data = other.waveform_data;
        waveform_size = other.waveform_size;
        //reset waveform_data and waveform_size - temporary object
        other.waveform_data = nullptr;
        other.waveform_size = 0;
    }
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}