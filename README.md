Warning: This animation conversion library is in early alpha and is not in a state that is considered stable, consistent, or tested. Use at your own risk and do not use for content that you will be releasing.



The intended method for use in animation conversion is to use an intermediary animation format that we are calling the Universal Animation format (within the CALUMI namespace).

For SFBGS .af + skeleton.rig import
-----------------------------------
1. Create an AnimationScene using the CALUMI::SFBGS namespace. (eg. ...::AnimationScene myStarfieldScene;)
2. This animation scene will consist of a SkeletonRig and a collection of Animations, however for now it will be empty.
3. Create a SkeletonRig under the same namespace, CALUMI::SFBGS (eg. ...::SkeletonRig myStarfieldRig;)
4. populate the rig with your chosen file with: auto result = myStarfieldRig.ReadFromFile("filepath.rig");
5. CALUMI.Animation uses the std::expected type to handle errors, if !result.has_value(), then you've encountered an error and can view that error with result.error().ToString();
6. If has_value() is true, you can now assign myStarfieldRig to myStarfieldScene.rig
7. Next you can iterate over a list of filepaths and create an SFBGS Animation called myStarfieldAnim (very creative name I know).
8. Follow a similar process as above with myStarfieldAnim.ReadFromFile("filepath.af");
9. For the animation list use myStarfieldScene.push_back(myStarfieldAnim);
10. You can place that into a loop to keep adding to the collection of animations.
11. Finally, create a universal AnimationScene under the CALUMI::UNIV called myUnivScene (namespaces are important in this library as many elements under different formats utilize the same phrases)
12. Convert the SFBGS scene and assign it to your new universal scene as so myUnivScene = myStarfieldScene.ConvertToUniversalScene();
13. You can now access each animation and its sequences in a more readible format. Vector3D's (doubles) are used for translations while the DirectX::SimpleMath library is currently used for Quaternions and Vector3's (for skeleton positioning to keep consistent data sizing with the file af format)


For UNIVERSAL creation and conversion to SFBGS .af's
----------------------------------------------------
1. This process is nearly identical but you can create the universal animation scene elements from an application like blender if you'd like
2. Create animations and add animation blocks to them using push_back
3. An animation block is a collection of sequences applied to some bone/limb/node/etc. Many systems use different methods of tracking these applied curves so we will want to use the corrent bone name and also the proper index. The root index will always be 0. I would add them in proper index order as well. If you are trying to match a starfield rig, you should look at the rig's string section at the bottom of the file to ensure the order is correct, because I've seen inconsistencies with the human skeleton.nif that was given  
4. Each animation block consists of four sequences. Translation, rotation, scale, and priority (as of now that priority isn't really understood completely so results may vary as it's just a 1:1 translation)
5. Each element of a sequence is made of a struct with a frame and an entry value (ie frame = 12, Translation = {x,y,z} as a DirectX::SimpleMath Vector3)
6. Create a universal scene and push_back each animation into the animations collection
7. Give your animation a title and apply the proper boneCount value for the amount of animation blocks you've added. For converting to SFBGS af's this count isn't as important but we should be consistent for the time being
8. It is required that you have a skeleton/rig to convert into SFBGS animations. Even if you are using an skeleton.rig that already exists and will not be writing a new one to file.
9. When you have your Universal AnimationScene created and you are satisfied, you can create an SFBGS AnimationScene and use myStarfieldScene.ConvertFromUniversalScene(myBlenderUniversalScene);
10. you can then iterate over each animation in the collection and use auto result = mySFBGSAnimation.WriteToFile("somenewfilepath.af");
11. As in the import section, if resulst.has_value() is true, then you are in business and it wrote "successfully" (results may vary lol). If it isn't true, then you can check the error message with result.error().ToString();
12. Be carefule with paths, they should be absolute paths and you should be careful to not overwrite a vanilla file. I would recommend doing a user check in your application for overwrite as this library doesn't give that consideration

Have fun and good luck! I'm sure we will find tons of bugs and headaches yippeee
