# 3D Slicer Reconstruct CT/MRI - Grandyang - 博客园







# [3D Slicer Reconstruct CT/MRI](https://www.cnblogs.com/grandyang/p/7740264.html)







3D Slicer Reconstruct CT/MRI

1. Load **DCM** file of your CT/MRI

2. Go to **Volume Rendering**, click the eye button to show volume

3. Go to **Editor** -> **Threshold Effect** -> Change **Threshold** (only include the head) -> **Apply**

4. Go to **Editor** - > **Make Model Effect** -> **Apply**

5. Go to **Volume Rendering**, click the eye button to hide volume

6. Go to **Models**, find the reconstructed model



Remove Redundant Points in MeshLab

1. Load model into MeshLab

2. Go to **Edit** -> **Select Connected Components in a region**.

3. Select some points on the part you want to keep, MeshLab will automatically select all the connected components.

4.Go to **Filters** -> **Selection** ->** Invert Selection**

5. Press **Delete** button on your keyboard

6. Go to **File** -> **Export Mesh As...** to save the clean model



Reference:

[https://www.youtube.com/watch?v=MKLWzD0PiIc](https://www.youtube.com/watch?v=MKLWzD0PiIc)














