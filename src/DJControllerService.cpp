#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    if(cache.contains(track.get_title())){//hit case
        cache.get(track.get_title());//the cache updates it's access time
        return 1;
    }
    AudioTrack* clone_track=track.clone().release();//miss case
    if(clone_track==nullptr){
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return 0;//since no evicition was done
    }
    clone_track->load();//simulating..
    clone_track->analyze_beatgrid();
    PointerWrapper<AudioTrack> clone_pointer_wrapper(clone_track);
    bool evicted=cache.put(std::move(clone_pointer_wrapper));
    if(evicted){
        return -1;
    }
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack* track = cache.get(track_title);
    if (track!=nullptr){
        return track;
    }
    return nullptr; 
}
