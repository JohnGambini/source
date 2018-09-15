#include "gnamidi.h"

gnaMidi::gnaMidi() : gnaTreeItem()
{
}

gnaMidi::gnaMidi(vector<gnaVariant> & items) : gnaTreeItem(items, 0)
{
}

int gnaMidi::parse(char const * midiData, unsigned long size )
{
    unsigned long index = 0;
    gnaint16 fileFormat = 0;
    gnaint16 trackCount = 0;
    gnaint16 ticks = 0;
    gnaint16 trackSize = 0;
    gnaint32 deltaTime = 0;
    gnaint8 midiCommand = 0;
    gnaint8 midiChannel = 0;
    gnaint8 commandByte = 0;
    gnaint8 ffLength = 0;
    gnaint8 ffCommand = 0;
    string dumpIt;
    //gnaTreeItem * trackItem = 0;

    do
    {
        if(midiData[index] == 'M') //This is either a file header or a track header
        {
            index += 2;
            if(midiData[index] == 'h') //It's a file header
            {
                index += 6; //Move past header ID and length

                fileFormat = midiData[index++];
                fileFormat = fileFormat << 8;
                fileFormat += midiData[index++];

                trackCount = midiData[index++];
                trackCount = trackCount << 8;
                trackCount += midiData[index++];

                ticks = midiData[index++];
                ticks = ticks << 8;
                ticks += midiData[index++];
            }
            else //It's a track header
            {
                index += 2;
                trackSize = midiData[index++];
                trackSize = trackSize << 8;
                trackSize += midiData[index++];
                trackSize = trackSize << 8;
                trackSize += midiData[index++];
                trackSize = trackSize << 8;
                trackSize += midiData[index++];

                //trackItem = gnaTreeItem
            }

        }
        else //It's a midi event
        {
            deltaTime = midiData[index] & 0x7F;
            while(( midiData[index++] & 0x80) != 0 )
            {
                deltaTime = deltaTime << 7;
                deltaTime += midiData[index] & 0x7F;
            }

            //The impressive and confusing midi command byte
            commandByte = midiData[index++];

            if((commandByte & 0x80) != 0) //We're not in running mode so don't use the previous midiCommand
            {
                midiCommand = (commandByte >> 4) & 0x0F;
                midiChannel = (commandByte & 0x0F);
            }

            switch(midiCommand)
            {
            case 0x08:  //Note off nn vv    (note number and velocity)
                index += 2;
                break;
            case 0x09:  //Note on nn vv     (note number and velocity)
                index += 2;
                break;
            case 0x0A:  //Key after-touch nn vv (note number and velocity )
                index += 2;
                break;
            case 0x0B:  //Control change cc vv  (controller number and new value)
                index += 2;
                break;
            case 0x0C:  //Program (patch) change pp (patch number)
                index ++;
                break;
            case 0x0D:  //Channel after touch cc (channel number)
                index ++;
                break;
            case 0x0E:  //Pitch wheel change bb tt (bottom 7 bits and top 7 bits)
                index += 2;
                break;
            case 0x0F:  //meta-event or system exclusive
                switch(commandByte)
                {
                case 0xF8:  //Timming clock used when sync required
                    break;
                case 0xF9:  //Nothing
                    break;
                case 0xFA:  //Start current sequence
                    break;
                case 0xFB:  //Continue
                    break;
                case 0xFC:  //Stop
                    break;
                case 0xFD:  //Nothing
                    break;
                case 0xFE:  //Nothing
                    break;
                case 0xFF:  //meta event
                    ffCommand = midiData[index++];
                    ffLength = midiData[index++];
                    //dumpIt.assign(&midiData[index],ffLength);
                    //cout << dumpIt.c_str() << "\n";
                    index += ffLength;
                    break;
                };
                break;
            };
        }

    }while(index < size);

    return 1;
}

