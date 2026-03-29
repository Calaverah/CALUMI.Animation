#pragma once

/*!
 * @page sfbgs_notes_page SFBGS Decoding Notes and Guidance
 *
 * This page contains general notes and tutorials for Starfield.
 *
 * @note Not all sections of the code have been decoded.
 * @warning When a new section is decoded, simply pluging in functionality into the animation class/structs is not sufficient. The goal is to create a universal interface for multiple games to be supported not just starfield. When in doubt, if it can't be used by another game, it shouldn't be implemented anywhere but the SFBGS namespace, with some exceptions...
 * 
 * 
 * @section sfbgs_anim_header Animation Header Notes
 * 
 * @details In the header lies a few peculiarities.
 * - Possible offset vectors for position and rotation/heading
 * - An unknown array of (floats?) that mostly appear to be filler, but that is not confirmed. A lot of the time something like -0.0f is displayed in the middle most index (3 total)
 * - Version number appears to be set to 5 however the Creation Kit can technically load a Version 4, at least in one line of code.
 * 
 * 
 * @section sfbgs_anim_flags Animation Header Flags
 * 
 * @details A small amount of bits take up 4 bytes in the header with the following *known* flags:
 * - Flag 1: Not Used
 * - Flag 2: Whether animation block counters use two bytes, one byte if set to false
 * - Flag 3: Whether animation block key frames use two bytes, one byte if set to false
 * - Flag 4: Whether scalars are present in the file
 * 
 * The remaining bits are either unused at present, or are simply padding. Total size is 4 bytes
 *
 * 
 * @section sfbgs_anim_preamble "Preamble"
 * 
 * @details This is an unknown section of data, in which we know the structure, but not the purpose or how it is used, without proof that is.
 * In the past there were thought of it being related to blending, however that doesn't appear to be the case and blending seems to simply be handling int he behavior graphs (agx)
 * The current working theories are as such:
 * - Animation driven sync data (Current winning theory)
 * - Animation driven variables (less likely with the BGSSequence forms)
 * 
 * @section sfbgs_anim_blocks Animation Blocks
 * 
 * @details Animation Blocks are the working name for the sections of data that carry keyframe sequences. 
 * The reason this name was chosen was to make a clear distinction between how we organize our decoded file structure and what is used in game. 
 * The game/BGS may or may not refer to these vectors as animation tracks, it isn't clear, and it's best to avoid naming conflicts in this case.\n \n 
 * In the animation block, rotation and translation sequences are broken up into three arrays of data. 
 * The first array are the keyframes to be considered for each entry. The next two arrays are high and low precision values, where the larger value (lower precision) features an RLE compression scheme. 
 * Each small value (high precision) is considered once while the larger value is repeated for a certain amount called out by it's count entry. 
 * Here we refer to each small value as entry, and each large value as prefix.
 * 
 * @subsection Rotation Rotation
 * @image html SFBGS_RotationScheme_Formulas.png "SFBGS Rotation Scheme Description"
 * 
 * @subsection Translation Translation
 * @details Translation compression is more simple than rotations however there are still some factors to consider when converting the values. The prefix and entry values are scaled by precision multipliers called out in the skeleton rig to which the animation is applied. For this reason, among others, skeleton rig references are required for converting translations.
 * 
 * @subsection Scalar Scalar
 * What's known:
 * - Each scalar entry is on a scale of 5000 units, where 5000 is 1.0
 * - Can be negative
 * - Can theoretically range from -32,768 to 32,767 or -6.5536 to 6.5534
 * - If present, the appropriate header flag MUST be set to true or the game will not only ignore the scalar sequence, but read it in as the next expected set of data with severe consequences
 * 
 * @subsection Priority Priority
 * @details Not much is known about this entry except that values seen in the vanilla game files range from 20-90 in increments of 10. At present it is believed to be used when additive animations are combined, taking the higher priority block for animating a rig's bone
 * 
 */