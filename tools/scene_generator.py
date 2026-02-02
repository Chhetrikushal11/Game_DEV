#!/usr/bin/env python3
"""
3D Scene Generator - Ancient Ruins
"""
import random
import math

print("Starting the scene Generator.....")

class SceneObject:
    def __init__(self, name, position, rotation_axis, rotation_angle, scale):
        self.name = name
        self.position = position
        self.rotation_axis = rotation_axis
        self.rotation_angle = rotation_angle
        self.scale = scale
    
    def to_cpp(self):
        code = f"    // {self.name}\n"
        code += f"    auto {self.name} = _mScene->CreateGameObject(\"{self.name}\");\n"
        code += f"    {self.name}->AddComponent(new MeshComponent(_mMaterial, _mMesh));\n"
        
        pos_x, pos_y, pos_z = self.position
        code += f"    {self.name}->SetPosition(glm::vec3({pos_x:.1f}f, {pos_y:.1f}f, {pos_z:.1f}f));\n"
        
        axis_x, axis_y, axis_z = self.rotation_axis
        angle = self.rotation_angle
        code += f"    {self.name}->SetRotation(glm::angleAxis(glm::radians({angle:.1f}f), glm::vec3({axis_x:.1f}f, {axis_y:.1f}f, {axis_z:.1f}f)));\n"
        
        scale_x, scale_y, scale_z = self.scale
        code += f"    {self.name}->SetScale(glm::vec3({scale_x:.1f}f, {scale_y:.1f}f, {scale_z:.1f}f));\n\n"
        
        return code

class SceneGenerator:
    def __init__(self):
        self.objects = []
        self.scene_name = "Exploration Sandbox"
    
    def add_ground_plane(self, size=20.0, height=-5.0):
        ground = SceneObject(
            name="ground",
            position=(0.0, height, 0.0),
            rotation_axis=(0.0, 1.0, 0.0),
            rotation_angle=0.0,
            scale=(size, 0.5, size)
        )
        self.objects.append(ground)
        print(f"  ✅ Added ground plane ({size}x{size})")
        return self
    
    def add_scattered_cubes(self, num_cubes=10, spread=15.0, height_range=(-2.0, 8.0)):
        for i in range(num_cubes):
            x = random.uniform(-spread, spread)
            y = random.uniform(height_range[0], height_range[1])
            z = random.uniform(-spread, spread)
            
            angle = random.uniform(0, 360)
            axis_choice = random.randint(0, 2)
            if axis_choice == 0:
                axis = (1.0, 0.0, 0.0)
            elif axis_choice == 1:
                axis = (0.0, 1.0, 0.0)
            else:
                axis = (0.0, 0.0, 1.0)
            
            scale_factor = random.uniform(0.5, 2.0)
            scale = (scale_factor, scale_factor, scale_factor)
            
            cube = SceneObject(
                name=f"rubble_{i}",  # Changed from "cube" to "rubble" for ruins theme
                position=(x, y, z),
                rotation_axis=axis,
                rotation_angle=angle,
                scale=scale
            )
            self.objects.append(cube)
        
        print(f"  ✅ Added {num_cubes} scattered rubble pieces")
        return self
    
    def add_tower(self, base_position, num_cubes=5):
        base_x, base_y, base_z = base_position
        
        # ✅ Create safe variable names (no negative signs)
        x_str = f"n{int(abs(base_x))}" if base_x < 0 else f"p{int(base_x)}"
        z_str = f"n{int(abs(base_z))}" if base_z < 0 else f"p{int(base_z)}"
        
        for i in range(num_cubes):
            tower_cube = SceneObject(
                name=f"pillar_{x_str}_{z_str}_lvl{i}",  # Changed to "pillar" for ruins
                position=(base_x, base_y + i * 2.0, base_z),
                rotation_axis=(0.0, 1.0, 0.0),
                rotation_angle=i * 15.0,
                scale=(0.8, 0.8, 0.8)
            )
            self.objects.append(tower_cube)
        
        return self
    
    def generate_cpp_code(self):
        code = "// ==========================================\n"
        code += f"// {self.scene_name}\n"
        code += f"// Generated {len(self.objects)} objects\n"
        code += "// ==========================================\n\n"
        
        for obj in self.objects:
            code += obj.to_cpp()
        
        return code
    
    def save_to_file(self, filename="generated_scene.cpp"):
        with open(filename, 'w') as f:
            f.write(self.generate_cpp_code())
        print(f"\n💾 Scene saved to {filename}")
        print(f"📦 Total objects: {len(self.objects)}")

def create_ancient_ruins():
    """Ancient Ruins - Broken pillars scattered across a desert"""
    print("\n🏛️ Creating Ancient Ruins...")
    scene = SceneGenerator()
    scene.scene_name = "Ancient Ruins"
    
    # Large desert ground
    scene.add_ground_plane(size=40.0, height=-5.0)
    
    # Broken pillars at various positions
    print("  🏛️ Adding broken pillars...")
    pillar_count = 0
    for x in range(-15, 16, 5):  # From -15 to 15, every 5 units
        for z in range(-15, 16, 5):
            # Random height for each pillar (some tall, some short/broken)
            height = random.randint(1, 4)  # 1-4 cubes high
            scene.add_tower((x, -4, z), num_cubes=height)
            pillar_count += 1
    
    print(f"  ✅ Added {pillar_count} broken pillars")
    
    # Scattered rubble (fallen stones)
    scene.add_scattered_cubes(num_cubes=30, spread=18.0, height_range=(-4.5, 1.0))
    
    return scene

if __name__ == "__main__":
    print("\n" + "="*50)
    print("🏛️ ANCIENT RUINS SCENE GENERATOR")
    print("="*50)
    
    scene = create_ancient_ruins()
    scene.save_to_file("generated_scene.cpp")
    
    print("\n" + "="*50)
    print("✅ DONE!")
    print("="*50)
    print("\n📋 Next steps:")
    print("  1. Open generated_scene.cpp")
    print("  2. Copy ALL the code")
    print("  3. Paste into Game.cpp Init() function")
    print("     (after creating _mMaterial and _mMesh)")
    print("  4. Update camera position:")
    print("     camera->SetPosition(glm::vec3(0.0f, 10.0f, 25.0f));")
    print("  5. Build and explore the ruins!")
    print()