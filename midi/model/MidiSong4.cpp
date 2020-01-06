
#include "MidiLock.h"
#include "MidiSong4.h"

void MidiSong4::assertValid() const
{
    // TODO: add stuff
}

void MidiSong4::addTrack(int trackIndex, int sectionIndex,  MidiTrackPtr track)
{
    if (trackIndex < 0 || trackIndex >= numTracks || sectionIndex < 0 || sectionIndex >= numSectionsPerTrack) {
        assert(false);
        return;
    }
    tracks[trackIndex][sectionIndex] = track;
}

MidiTrackPtr MidiSong4::getTrack(int trackIndex, int sectionIndex)
{
     if (trackIndex < 0 || trackIndex >= numTracks || sectionIndex < 0 || sectionIndex >= numSectionsPerTrack) {
        assert(false);
        return nullptr;
    }
    return tracks[trackIndex][sectionIndex];
}


 MidiSong4Ptr MidiSong4::makeTest(MidiTrack::TestContent content, int trackIndex, int sectionIndex)
 {
    MidiSong4Ptr song = std::make_shared<MidiSong4>();
    MidiLocker l(song->lock);
    auto track = MidiTrack::makeTest(content, song->lock);
    song->addTrack(trackIndex, sectionIndex, track);
    song->assertValid();
    return song;
 }

 float MidiSong4::getTrackLength(int trackIndex) const
 {
     if (trackIndex < 0 || trackIndex >= numTracks ) {
        assert(false);
        return 0;
    }
    float ret = 0;
    for (auto section : tracks[trackIndex]) {
        if (section) {
            ret += section->getLength();
        }
    }
    return ret;
 }

  void MidiSong4::createTrack(int index, int sectionIndex)
  {
    assert(lock);
    addTrack(index, sectionIndex, std::make_shared<MidiTrack>(lock));
  }

  void MidiSong4::_flipTracks()
  {
      std::swap(tracks[0], tracks[1]);
  }