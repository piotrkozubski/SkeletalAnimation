# Skeletal animation exporter to PMESH format
# The format stores information of triangle mesh: 
# - vertex position mandatory
# - normal, UV, 2 * bones index and weight optional
# If bones are attached to vertices, then animation of bones must be defined.
# So models can be exported in two ways: 
# - static mesh without bones
# - mesh with animated bones
# Bone rules:
# - must be created in order from root to child - no child can have index 
#   smaller that its ancestors. This is mandatory, because for every key frame, 
#   list of changed bones is sorted in order from child to parent.
# - animation should have at least 2 KEY frames, at first (0) index and last index
# - there musn't be key frames out of total frames
# - for every key frame, every bone's interpolation must be generated 
#   (select all bones in POSE mode, press I and select WHOLE CHARACTER)
#   if bone has no change in key frame, it will be skipped by script
# - vertices must be assigned to vertex groups which names correspond to bones 
#   names (exactly the same names)
# - bones changes should be described only by quaternion (root have vec3 for location)
# - data_path objects of fcurves, should fullfill following pattern:
#   1) pose.bone["NAME OF THE BONE"].location - this is location, saved only for root bone
#   2) pose.bone["NAME OF THE BONE"].rotation_quaternion - for root and all other

import bpy
import mathutils
import math

bl_info = {
    "name": "PMESH exporter",
    "author": "Piotr Kozubski",
    "version": (0, 5, 6, 6),
    "blender": (2, 7, 2),
	"location": "File > Export > PMESH exporter",
	"category": "Import-Export",
}

class VertexType:
    x = 0.0
    y = 0.0
    z = 0.0
    nx = 0.0
    ny = 0.0
    nz = 0.0
    u = 0.0
    v = 0.0
    bone1I = -1
    bone1W = 0
    bone2I = -1
    bone2W = 0
    bone3I = -1
    bone3W = 0
    bone4I = -1
    bone4W = 0
    d = 0
    
class PolygonType:
    v = []
    
class BoneQuat:
    t = [0, 0, 0]  # translation 0-2
    q = [1, 0, 0, 0]  # quaternion 0-3
    
class Bone:
    id = -1
    keyFramesDict = {}  # key = frame id, value = BoneQuat object
    
def getBoneNameFromFC(fcurve):
    list = fcurve.data_path.split('"')
    return list[1]
    
def findBoneIndex(groupName, bones):
    for idx, bone in enumerate(bones):
        if bone.name == groupName:
            return idx
    print("ERROR, cannot find bone id for vertex! groupName: " + groupName);
    return 0
    
def rotateQuatAxis(rot_mat, quat):
    #print("input matrix " + str(rot_mat))
    #print("input quat " + str(quat))
    
    rot_quat = rot_mat.to_quaternion()

    if quat[0] > 1.0:
        quat[0] = 1.0
    if quat[0] < -1.0:
        quat[0] = -1.0
    angle = math.acos(quat[0]) * 2
    angle_deg = angle * 57.2957795
    sin_half = math.sin(angle / 2)
    if (sin_half != 0):
        axis_x = quat[1] / sin_half
        axis_y = quat[2] / sin_half
        axis_z = quat[3] / sin_half
    else:
        axis_x = 0
        axis_y = 0
        axis_z = 0
    
    #print("unpacked axis: " + str(axis_x) + ", " + str(axis_y) + ", " + str(axis_z) + ", " + str(angle_deg))
    
    v = mathutils.Vector((axis_x, axis_y, axis_z))
    new_axis = rot_mat * v
    
    return mathutils.Quaternion(new_axis, angle)
    
    
def findIndex(list, lookFor):
    for elIdx, element in enumerate(list):
        if element == lookFor:
            return elIdx
    return -1
    
def printVertAndFace(f, vertexList, polygonList, isArmature):
    f.write("#number of vertex\n")
    f.write("%d\n" % (len(vertexList)))     #vertices num
    f.write("#number of triangles\n")
    f.write("%d\n" % len(polygonList))    #faces num
    f.write("#normal flag\n")
    f.write("1\n")                                #normals flag
    f.write("#texture coordinates flag\n")
    f.write("1\n")                                #texture coord num
    if isArmature:
        f.write("#bones flag\n")
        f.write("1\n")                                # bones flag
    else:
        f.write("#bones flag\n")
        f.write("0\n")
    f.write("#vertex list (x, y, z, nx, ny, nz, u, v, b1 I, b2 I, b1 W, b2 W)\n")
    for v in vertexList:
        if v.d == 0:
            f.write("%.10f, " % v.x)
            f.write("%.10f, " % v.y)
            f.write("%.10f, " % v.z)
            f.write("%.10f, " % v.nx)
            f.write("%.10f, " % v.ny)
            f.write("%.10f, " % v.nz)
            f.write("%.10f, " % v.u)
            f.write("%.10f, " % v.v)
            if isArmature:
                f.write("%d, " % v.bone1I)
                f.write("%d, " % v.bone2I)
                f.write("%d, " % v.bone3I)
                f.write("%d, " % v.bone4I)
                f.write("%.10f, " % v.bone1W)
                f.write("%.10f, " % v.bone2W)
                f.write("%.10f, " % v.bone3W)
                f.write("%.10f" % v.bone4W)
            f.write("\n")
    f.write("#triangle list (vertex 1 index, vertex 2 index, vertex 3 index)\n")
    for p in polygonList:
        f.write("%d, " % p.v[0])
        f.write("%d, " % p.v[1])
        f.write("%d\n" % p.v[2])
    
def VertexEqual(vertexList, idx, idxDup):
    print("VertexEqual: " + ", " + str(idx) + ", " + str(idxDup))
    result = (vertexList[idx].x == vertexList[idxDup].x) and (vertexList[idx].y == vertexList[idxDup].y) and (vertexList[idx].z == vertexList[idxDup].z) and (vertexList[idx].u == vertexList[idxDup].u) and (vertexList[idx].v == vertexList[idxDup].v)
    print("result = " + str(result));
    return result
    
def ItemVertexEqual(vertexList, v, vDup):
    print("ItemVertexEqual: " + str(vertexList.index(v)) + ", " + str(vertexList.index(vDup)))
    result = (v.x == vDup.x) and (v.y == vDup.y) and (v.z == vDup.z) and (v.u == vDup.u) and (v.v == vDup.v)
    if (result):
        print("result = " + str(result));
    return result
    
def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

def write_some_data(context, filepath, use_some_setting):
    print("running write_some_data...")
    f = open(filepath, 'w', encoding='utf-8')
    #f.write("#PMESH generated by Blender export addon\n")

    # FIND ARMATURE
    armature = 0
    for arm in bpy.context.scene.objects:
        if arm.type != 'ARMATURE':
            print("Looking for Armature object...")
        else:
            print("Armature found!")
            armature = arm
            break
    
    # need to check if animation defined
    if armature:
        if not hasattr(armature.animation_data, 'action'):
            raise Exception('No animation assigned to the armature!')
        SaveBones = (armature != 0 and armature.animation_data.action != None)    
    else:
        SaveBones = 0
    
    # =============== MESHES ====================
    item = bpy.context.scene.objects.active.copy()
    if item.type != 'MESH':
        print("Object type must be 'MESH'")
    else:
        print("item name: " + item.name)
        #triangulate faces of the mesh
        mesh_triangulate(item.data)

        # Be sure tessface & co are available!
        item.data.calc_tessface()
        
        # get object world matrix
        matrix_world = item.matrix_world
        
        # COLLECTING VERTICES, FACES
        vertexList = []
        polygonList = []
        dict = {}
        vertex_counter = 0
        for polIdx, face in enumerate(item.data.polygons):
            vIdx = []
            for vert, loop in zip(face.vertices, face.loop_indices):
            # vertex
                v = VertexType()
                final_v = matrix_world * item.data.vertices[vert].co;
                v.x = final_v[0]
                v.y = final_v[1]
                v.z = final_v[2]
            # normal
                v.nx = item.data.vertices[vert].normal[0]
                v.ny = item.data.vertices[vert].normal[1]
                v.nz = item.data.vertices[vert].normal[2]
            # uv
                if (len(item.data.uv_layers)):
                    v.u = item.data.uv_layers.active.data[loop].uv[0]
                    v.v = item.data.uv_layers.active.data[loop].uv[1]
            # bone 1
                vertex = item.data.vertices[vert]
                if len(vertex.groups) >= 1: # if there is at least 1 bone attached
                    v.bone1I = findBoneIndex(item.vertex_groups[vertex.groups[0].group].name, armature.data.bones)
                    #v.bone1I = vertex.groups[0].group
                    v.bone1W = vertex.groups[0].weight
            # bone 2
                if len(vertex.groups) >= 2: # if there are at least 2 bones attached
                    v.bone2I = findBoneIndex(item.vertex_groups[vertex.groups[1].group].name, armature.data.bones)
                    #v.bone2I = vertex.groups[1].group
                    v.bone2W = vertex.groups[1].weight
            # bone 3
                if len(vertex.groups) >= 3: # if there are at least 3 bones attached
                    v.bone3I = findBoneIndex(item.vertex_groups[vertex.groups[2].group].name, armature.data.bones)
                    #v.bone3I = vertex.groups[2].group
                    v.bone3W = vertex.groups[2].weight
            # bone 4
                if len(vertex.groups) >= 4: # if there are at least 4 bones attached
                    v.bone4I = findBoneIndex(item.vertex_groups[vertex.groups[3].group].name, armature.data.bones)
                    #v.bone4I = vertex.groups[3].group
                    v.bone4W = vertex.groups[3].weight
                if len(vertex.groups) > 4:
                    print("Warning! Vertex " + str(vert) + " has " + str(len(vertex.groups)) + " bones attached.");

                key = v.x, v.y, v.z, v.u, v.v
                if dict.get(key) == None:
                    dict[key] = vertex_counter
                    vertex_counter = vertex_counter + 1
                    vertexList.append(v)
                vIdx.append(dict[key])
            
            p = PolygonType()
            p.v = vIdx
            polygonList.append(p)


        # EXPORTING VERTICES, FACES TO FILE
        #f.write("%d\n" % (len(vertexList) - deletedV))     #vertices num
        printVertAndFace(f, vertexList, polygonList, SaveBones)
            
    # ============== BONES ==================
    if SaveBones:
        bonesDict = {} # key = bone name, value = Bone object        
        bm_world = armature.matrix_world
        # number of bones
        f.write("#number of bones\n")
        f.write("%d\n" % len(armature.data.bones))
        f.write("#bone list (parent, 4x4 matrix)\n")
        for idx, bone in enumerate(armature.data.bones):
            b = Bone()
            b.id = idx
            b.keyFramesDict = {}
            bonesDict[bone.name] = b
            print("bone name: " + bone.name)
            
            f.write("%d, " % (findIndex(armature.data.bones, bone.parent) if bone.parent != None else -1))
            m = bone.matrix_local
            w = bm_world * m
#            f.write("%.10f, %.10f, %.10f, %.10f, " % (w[0][0], w[0][1], w[0][2], w[0][3]))
#            f.write("%.10f, %.10f, %.10f, %.10f, " % (w[1][0], w[1][1], w[1][2], w[1][3]))
#            f.write("%.10f, %.10f, %.10f, %.10f, " % (w[2][0], w[2][1], w[2][2], w[2][3]))
#            f.write("%.10f, %.10f, %.10f, %.10f\n" % (w[3][0], w[3][1], w[3][2], w[3][3]))
            
            f.write("1.0, 0.0, 0.0, %.10f, " % (w[0][3]))
            f.write("0.0, 1.0, 0.0, %.10f, " % (w[1][3]))
            f.write("0.0, 0.0, 1.0, %.10f, " % (w[2][3]))
            f.write("0.0, 0.0, 0.0, 1.0 #" + bone.name)
            f.write("\n")
            
        # =========== BONES ANIMATION =============
        #f.write("#start frame\n")
        #f.write("%d\n" % armature.animation_data.action.frame_range.x)
        f.write("#number of frames\n")
        f.write("%d\n" % (armature.animation_data.action.frame_range.y + 1))
        
        # ============ BONES KEY FRAMES ===========
        keyFramesNum = 0
        for fcurveIdx, fcurve in enumerate(armature.animation_data.action.fcurves):
            #if fcurve.group == None:
            #    print("ERROR: fcurve idx " + str(fcurveIdx) + " is none\n")
            #    return
            #if bonesDict.get(fcurve.group.name) == None:
            #    print("ERROR: " + fcurve.group.name + " bone not found\n")
            #    return
            #print("found keyframe for bone: " + fcurve.group.name)
            #bone = bonesDict[fcurve.group.name]
            bone = bonesDict[getBoneNameFromFC(fcurve)]
            
            for frame in fcurve.keyframe_points:
                if bone.keyFramesDict.get(frame.co.x) == None:
                    bone.keyFramesDict[frame.co.x] = BoneQuat()
                    bone.keyFramesDict[frame.co.x].t = [0, 0, 0]
                    bone.keyFramesDict[frame.co.x].q = [1, 0, 0, 0]
                    keyFramesNum = keyFramesNum + 1
                
                # in fact translate offset is needed only for root bone
                if fcurve.data_path.endswith('.location'):
                    bone.keyFramesDict[frame.co.x].t[fcurve.array_index] = frame.co.y
                    
                elif fcurve.data_path.endswith('.rotation_quaternion'):
                    bone.keyFramesDict[frame.co.x].q[fcurve.array_index] = frame.co.y

        # write BONES key frames
        # count valid key frames
        keyFramesNum = 0
        identFrames = 0
        for bone in bonesDict.keys():
            b = bonesDict[bone]            
            for frame in b.keyFramesDict.keys():
                q = b.keyFramesDict[frame]
                if q.q[0] != 1.0 or q.q[1] != 0.0 or q.q[2] != 0.0 or q.q[3] != 0.0: # quat must rotate
                    keyFramesNum = keyFramesNum + 1
                else:
                    identFrames = identFrames + 1
        print("identity frames: " + str(identFrames))
        
        f.write("# number of key frames\n")
        f.write("%d\n" % keyFramesNum)
        f.write("# key frames (bone, key frame, [if root bone: x, y, z, offset] quaternion X, quaternion Y, quaternion Z, quaternion W)\n")
        for bone in bonesDict.keys():
            b = bonesDict[bone]
            matrix_inv = armature.data.bones[b.id].matrix_local
            
            for frame in b.keyFramesDict.keys():
                q = b.keyFramesDict[frame]
                
                if q.q[0] != 1.0 or q.q[1] != 0.0 or q.q[2] != 0.0 or q.q[3] != 0.0: # quat must rotate
                    f.write("%d, %d, " % (b.id, frame))
                    if b.id == 0: # write translate offset only for root bone
                        f.write("%.10f, %.10f, %.10f, " % (q.t[0], q.t[1], q.t[2]))

                    inverted_q = rotateQuatAxis(matrix_inv.to_3x3(), q.q)
                    #f.write("%.10f, %.10f, %.10f, %.10f\n" % (q.q[1] * -1, q.q[3], q.q[2], q.q[0]))
                    #f.write("%.10f, %.10f, %.10f, %.10f\n" % (q.q[1], q.q[2], q.q[3], q.q[0]))
                    f.write("%.10f, %.10f, %.10f, %.10f\n" % (inverted_q.x, inverted_q.y, inverted_q.z, inverted_q.w))
                

    f.close()
    print("done")

    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportSomeData(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_test.some_data"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Some Data"

    # ExportHelper mixin class uses this
    filename_ext = ".pmesh"

    filter_glob = StringProperty(
            default="*.pmesh",
            options={'HIDDEN'},
            maxlen=255,  # Max internal buffer length, longer would be clamped.
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting = BoolProperty(
            name="Example Boolean",
            description="Example Tooltip",
            default=True,
            )

    type = EnumProperty(
            name="Example Enum",
            description="Choose between two items",
            items=(('OPT_A', "First Option", "Description one"),
                   ('OPT_B', "Second Option", "Description two")),
            default='OPT_A',
            )

    def execute(self, context):
        return write_some_data(context, self.filepath, self.use_setting)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportSomeData.bl_idname, text="PMESH exporter")


def register():
    bpy.utils.register_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_test.some_data('INVOKE_DEFAULT')
