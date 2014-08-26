------------------------------------------------------------------------------------------------
-- Starcraft 2 Model (M3->3ds) Importer version 0.21
-- by NiNtoxicated (madyavic@gmail.com)
-- Imports M3 models into 3ds max via maxscript
-- Created for 3ds max 2010, but should be backwards compatible
-- Based on reworked code from my World of Warcraft M2 importer
--
-- Big thanks to: 
-- Teal (starcraft.incgamers.com) - PHP M3 parser
-- Volcore (http://volcore.limbicsoft.com/)  - Helped figure out vertex flags
-- Witchsong (http://code.google.com/p/libm3/) - Awesome M3 library and for help on sequence data
-- der_Ton (http://www.doom3world.org/) - Great work on MD5 format which is similar to M3
-- ufoZ - One of the original people to reverse engineer the M2 format and provide a good maxscript importer
-- 			from which my importer/exporters were originally based
-- Check out http://code.google.com/p/libm3/ for more indepth detail about the M3 format
-- Head to http://www.sc2mapster.com/ for more mod tools and Starcraft 2 content
--
-- If you use any of this code in your own scripts, please credit
-- where it is due.
--
-- Things to do:
-- 1. Clean up messy code
-- 2. Implement 3dsMax GUI properly
-- 3. Automate MPQ extraction
-- 4. Add good animation support
--
------------------------------------------------------------------------------------------------
--
-- Version History:
-- 0,21 -- May 7th 2010
-- Fixed wrong textures being applied to some models
-- Fixed missing materials dialog duplicate error
-- Added reflection map support
-- Added code to deal with vertex flags
-- Added vertex normals UI option
-- Specular level now being set from material data
--
-- 0.20 -- April 30th 2010
-- Added bindpose buffers for each animation, helps prevent weird translation across animations
-- Vertex normals now manually set properly (at the expense of processing time :( )
-- Code syntax improvements
--
-- 0.18 -- April 25th 2010
-- Added support for new MD34 model format
-- Bones now animate correctly
--
-- 0.14 -- April 10th 2010
-- Aligned all submeshes to their appropriate bones 
-- Skinned vertex positioning now based on all weighted bones (thanks to MD5 documentation)
-- Improved animation processing times (bones animated before vertex skinning)
--
-- 0.13 -- April 1st 2010
-- Mesh now binds to bones properly for most models.
--
-- 0.11 -- March 30th 2010
-- Updated Sequence support
-- Added Sequence UI for navigating sequences
-- Shift to more object orientated coding
--
-- 0.098 -- March 26th 2010
-- Added queryBox error for missing textures before creating any scene objects
-- Updated some of the code regarding sequence data
-- Minor code improvements
--
-- 0.096 -- March 22nd 2010
-- Fixed bump mapping (sets to 100 now)
-- 
-- 0.095 -- March 21st 2010
-- Some basic error catching added
-- Fixed initial bone errors
--
-- 0.09 -- March 20th 2010
-- Added primitive animation support
-- Code housekeeping
-- Updated code, improved processing time
--
-- 0.06 -- March 13th 2010
-- Added rollout menu
--
-- 0.05 -- March 11th 2010
-- Added bone support, however mesh not binding to bones properly yet
--
-- 0.04 -- March 6th 2010
-- Major overhaul of code
-- Reworked tag reading and reference reading, can now read nested structures
-- Vastly improved material reading
--
-- 0.03 -- March 4th 2010
-- Vertice normals added 
-- Added material importing
--
-- 0.02 -- February 29th 2010 - March 2nd 2010
-- Made significant improvements to structures and accessing file data
-- Can now (hopefully) import UV data
-- Added submesh functionality
--
-- 0.01 -- February 26th 2010
-- Initial version of M3 import script
--
------------------------------------------------------------------------------------------------

-- ********************
--  SCRIPT DECLARES
-- ********************
-- globals
global head, cmat, mread, aread, bstream, cbones
global scrStart, scrEnd -- For script timeStamps
global tread		= #()
global cmesh		= #()

-- Switches
global flipuv_y 				= true -- Flips uv.y coords
global scrExit					= false -- Indicates premature script exit

-- UI switches
global doCreateBones 		= true -- #warning: can slow import
global doTransformBones 	= true
global doCreateSkin 		= true
global doCreateMesh 		= true
global doCreateMats		= true
global doVertexNormals	= true -- #warning: slows import

-- Misc. variables
global bonesize 	= 0.010
global useFPS 	= 30

--Function declaration
fn ReadTagData tagInd flags = ()
fn M3I_AnimUI = ()

-- *******************
--  M3 STRUCTURES
-- ******************* 
struct M3I_SeqData
(
	tag, ablock
)

fn getSize type flags:0 =
(
	local size
	case type of
	(
		#REF: 
		(
			case flags of
			(
				#MD33: size = 0x08
				#MD34: size = 0x0C
			)
		)
		#MD33: size = 0x14
		#MODL:
		(
			case flags of
			(
				23: size = 0x240
			)
		)
		#SEQS: size = 0x58
		#STC_: size = 0x8C
		#STG_: size = 0x10
		#STS_: size = 0x18
		#BONE: size = 0x9C
		#VERT: size = 0x20
		#U16_: size = 0x02
		#U32_: size = 0x04
		#CHAR: size = 0x01
		#DIV_: size = 0x24
		#REGN: size = 0x1C
		#BAT_: size = 0x0E
		#MSEC: size = 0x48
		#MATM: size = 0x08
		#MAT_: size = 0xD4
		#LAYR: size = 0x160
		#IREF: size = 0x40
	)
	
	return size
)

-- ***********
--  HELPERS
-- ***********
-- Liberally stolen from my M2 script
fn echo msg =
(
	format "%\n" (msg) to:listener
)

fn ReadFixedString bitStream fixedLen=
(
	local str = ""
	for i = 1 to fixedLen do
	(
		str += bit.intAsChar (ReadByte bitStream #unsigned)
	)
	return str
)

fn ReadVec bitStream flag array:0 =
(
	local va = #()
	local vec = [0,0,0]
	
	-- Check if array of vectors or single vector
	if array == 0 then 
	(
		count = 1
	)
	else 
	(
		count = array
		-- Initialize array
		va[count] = 0
	)
	
	for i = 1 to count do
	(
		case flag of
		(
			#v3dshort: (vec.x = ReadShort bitStream #unsigned; vec.y = ReadShort bitStream #unsigned; vec.z = ReadShort bitStream #unsigned;)
			#v3dlong: (vec.x = ReadLong bitStream #unsigned; vec.y = ReadLong bitStream #unsigned; vec.z = ReadLong bitStream #unsigned;)
			#v3dbyte: (vec.x = ReadByte bitStream #unsigned; vec.y = ReadByte bitStream #unsigned; vec.z = ReadByte bitStream #unsigned;)
			#quat: (vec = quat 0 0 0 1; vec.x = ReadFloat bitStream; vec.y = ReadFloat bitStream; vec.z = ReadFloat bitStream; vec.w = ReadFloat bitStream;)
			#v4dshort: (vec = [0,0,0,0]; vec.x = ReadShort bitStream #signed; vec.y = ReadShort bitStream #signed; vec.z = ReadShort bitStream #signed; vec.w = ReadShort bitStream #signed;)
			#v4dbyte: (vec = [0,0,0,0]; vec.x = ReadByte bitStream; vec.y = ReadByte bitStream; vec.z = ReadByte bitStream; vec.w = ReadByte bitStream;)
			#v3d: (vec = [0.0,0.0,0.0]; vec.x = ReadFloat bitStream; vec.y = ReadFloat bitStream; vec.z = ReadFloat bitStream;)
			#matrix: 
			(
				vma = #()
				vma[4] = 0
				for j = 1 to 4 do
				(
					local vm = [0,0,0,0]
					vm.x = ReadFloat bitStream; vm.y = ReadFloat bitStream; vm.z = ReadFloat bitStream; vm.w = ReadFloat bitStream;
					vma[j] = vm
				)
				vec = matrix3 vma[1] vma[2] vma[3] vma[4]
			)
		)
		if (array != 0) then (va[i] = vec) else (va = vec)
	)
	
	return va
)

fn SkipBytes bitStream count =
(
	local unknown
	case count of
	(
		2: unknown = ReadShort bitStream #unsigned
		4: unknown = ReadLong bitStream #unsigned
		default:
		(
			for i = 1 to count do
			(
				unknown = ReadByte bitStream #unsigned
			)
		)
	)
)
	
fn LongToString num=
(
	local str = ""
	for i = 1 to 4 do
	(
		str += bit.intAsChar (bit.and num 0xff)
		-- num = bit.shift num -8
		num /= 256
	)
	str
)

fn ShortToFloat &val = -- Short to Float func
(
	if (val > 0) then (val -= 32767) else (val += 32767)
	val /= 32767.0
)

fn M3I_Convert_Time t =
(
	t / (1000 / useFPS)
)

fn M3I_GetID chunkid =
(
	local strID = chunkid as string
	local tagID = "BLNK" -- Needs to be 4 chars for modification
	for i = 1 to 4 do
	(
		k = 5 - i
		tagID[i] = strID[k]
	)
	id = toUpper tagID -- Uppercase
	return id
)

fn M3I_Reset_Globals =
(
	head = mread = bstream = cmesh = undefined
	cmat = #()
	aread = #()
	tread = #()
	cbones = #()
	gc()
)

-- ************************
-- M3 FUNCTIONS
-- ************************ 
fn M3I_Open fname flags =
(
	M3I_Reset_Globals()
	
	step = "Accessing Model File"
	
	echo ("Opening "+fname+"...")
	bitStream = fopen fname flags
	if bitStream==undefined then 
	(
		echo "File not found!"
		throw "File not found"
	)
	
	return bitStream
)

fn M3I_Close bitStream =
(
	step = "Close"
	fclose bitStream
)

struct M3I_Sphere
(
	vmin, vmax, rad, flags,
	
	fn Read bitStream flags =
	(
		local s = M3I_Sphere()
		s.vmin = ReadVec bitStream #v3d
		s.vmax = ReadVec bitStream #v3d
		s.rad = ReadFloat bitStream
		if (head.fileID == #MD34) then s.flags = ReadLong bitStream #unsigned
		
		return s
	)
)

struct M3I_Tag -- Tag data
(
	dataID, ofsData, nData, flags,
	
	fn Read bitStream hinfo =
	(
		step = "Read Tags"
		
		local bm = ftell(bitStream) -- bookmark
		-- Pre-initializing arrays speeds up script
		local tr = #()
		tr[hinfo.nTag] = 0
		if (fseek bitStream hinfo.ofsTag #seek_set) then
		(	
			for i=1 to hinfo.nTag  do
			(
				local t 		= M3I_Tag()
				local chunkid = (ReadFixedString bitStream 4)
				t.dataID 	= (M3I_GetID chunkid) as name
				t.ofsData	= ReadLong bitStream #unsigned
				t.nData		= ReadLong bitStream #unsigned
				t.flags		= ReadLong bitStream #unsigned
			
				tr[i] = t
			)
		)
		fseek bitStream bm #seek_set -- return to bookmark
	
		return tr
	)
)

struct M3I_Ref
(
	entries, refid, data = #(), flags,
	
	fn Read bitStream flags:0 =
	(
		local r = M3I_Ref()
		r.entries 	= ReadLong bitStream #unsigned
		r.refid		= ReadLong bitStream #unsigned + 1
		if (head.fileID == #MD34) then r.flags = ReadLong bitStream #unsigned
		if (flags != #nodata) then
		(
			r.data 		= ReadTagData bitStream r.refid flags
			return r.data
		)
		else return r
	)
)

struct M3I_AnimRef -- Animation Reference
(
	flags, animflags, animid,
	
	fn Read bitStream =
	(
		ar = M3I_AnimRef()
		ar.flags = ReadShort bitStream #unsigned
		ar.animflags = ReadShort bitStream #unsigned
		ar.animid = ReadLong bitStream #unsigned
		
		return ar
	)
)

fn M3I_Name name flt ext:undefined =
(
	-- Filter name from path
	local mn = filterString name flt
	mn = mn[mn.count]
	local mext = substring mn (mn.count-3) (4)
	-- Remove extension if specified
	if (ext != undefined) then
	(
		if (matchPattern mext pattern:ext ignoreCase:true) then mn = substring mn 1 (mn.count-4)
	)
	return mn
)

struct M3I_Header
(
	fileID, ofsTag, nTag, mref = M3I_Ref(),
	
	fn Read bitStream &header =
	(
		step = "Read header"
		echo "Reading Model Header..." 
	
		header = M3I_Header()
		local fileID = (ReadFixedString bitStream 4)
		header.fileID = (M3I_GetID fileID) as name
		echo ("Model Type: "+header.fileID as string)
		if (header.fileID != #MD33 and header.fileID != #MD34) then
		(
			echo "Not a valid Starcraft 2 model file!"
			throw "Not a valid Starcraft 2 model file!"
		)
		header.ofsTag 	= ReadLong bitStream #unsigned
		header.nTag		= ReadLong bitStream #unsigned
		header.mref		= M3I_Ref.Read bitStream flags:#nodata
	)
)

struct M3I_MODL
(
	name,  -- max model name
	versID, 
	SEQS, STC, STS, STG, -- anim info
	bones, 
	vflags, verts, DIV, blist,
    bndSphere,
	matm, mat,
	IREF
)

fn M3I_Read_MODL bitStream flags =
(
	echo "Reading Model Data..."
	mread				= M3I_MODL()
	local rsize			= getSize #Ref flags:head.fileID
	local mname 		= M3I_Ref.Read bitStream
	mread.name 		= M3I_Name mname "\\" ext:".max"
	echo ("Model Name: " + mread.name)	
	mread.versID 		= ReadLong bitStream #unsigned
	-- Animation data
	-- Don't read in unneccessary big chunks of data unless transforming bones
	if (doCreateBones and doTransformBones) then 
	(
		echo "Reading Animations..."
		mread.SEQS = M3I_Ref.Read bitStream
		-- Potentially massive array of data, better to store it in a different variable
		aread = M3I_Ref.Read bitStream
		mread.STG = M3I_Ref.Read bitStream
		
		echo "Animations read\n"
	)
	else
	(
		SkipBytes bitStream (rsize * 3)
	)
	
	-- SkipBytes
	case (head.fileID) of
	(
		#MD33: SkipBytes bitStream 0x14
		default: SkipBytes bitStream 0x1C
	)
	
	-- Bones
	if (doCreateBones) then
	(
		echo "Reading bones..."
		mread.bones		= M3I_Ref.Read bitStream
		echo "Bones read\n"
	)
	else
	(
		SkipBytes bitStream (rsize)
	)
	
	SkipBytes bitStream 0x04
	
	-- Mesh
	if (doCreateMesh) then
	(
		echo "Reading mesh data..."
		mread.vflags		= ReadLong bitStream #unsigned
		mread.verts		= M3I_Ref.Read bitStream flags:mread.vflags
		mread.DIV			= M3I_Ref.Read bitStream
		mread.blist			= M3I_Ref.Read bitStream flags:#ushort
		echo "Mesh data read\n"
	)
	else
	(
		SkipBytes bitStream (rsize * 4)
	)
	
	mread.bndSphere = M3I_Sphere.Read bitStream head.fileID
	
	case (head.fileID) of
	(
		#MD33: 
		(
			case flags of
			(
				20: SkipBytes bitStream 0x5C
				23: SkipBytes bitStream 0x64
			)
		)
		default:
		(
			case flags of
			(
				20: SkipBytes bitStream 0x1C
				23: SkipBytes bitStream 0x84
			)
		)
	)
		
	if (doCreateMats) then
	(
		echo "Reading materials..."
		mread.matm	= M3I_Ref.Read bitStream
		mread.mat		= M3I_Ref.Read bitStream
		echo "Materials read\n"
	)
	else
	(
		SkipBytes bitStream (rsize * 2)
	)
	
	-- SkipBytes
	case (head.fileID) of
	(
		#MD33: SkipBytes bitStream 0xA8
		default: SkipBytes bitStream 0xFC
	)
	
	if (doCreateSkin) then 
	(
		mread.IREF = M3I_Ref.Read bitStream
	)
	else
	(
		SkipBytes bitStream (rsize)
	)
)

fn M3I_Read_Start bitStream =
(
	step = "Read Model Data"
	scrExit = false -- reset script exit flag
	scrStart = TimeStamp() -- start tracking script processing time
	M3I_Header.Read bitStream &head
	tread = M3I_Tag.Read bitStream head
	mtag = tread[head.mref.refid]
	-- Position bitstream at modl entry... kind of redundant as we should be there already
	fseek bitStream mtag.ofsData #seek_set
	
	M3I_Read_MODL bitStream mtag.flags
)

struct M3I_AnimData
(
	d1 = #(), name, d2, maxframes, flags, d4 = #(), bndSphere,
	cstart, cend, -- Max variables
	
	fn Read bitStream tag =
	(
		ad = #()
		ad[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			a = M3I_AnimData()
			
			SkipBytes bitStream 8
			a.name = M3I_Ref.Read bitStream
			SkipBytes bitStream 4
			local mframes = ReadLong bitStream #unsigned
			a.maxframes = M3I_Convert_Time mframes
			SkipBytes bitStream 0x1C
			a.bndSphere = M3I_Sphere.Read bitStream head.fileID
			SkipBytes bitStream 0x08
			
			ad[i] = a
		)
		
		echo ("nAnims: " + ad.count as string)
		return ad
	)
)

struct M3I_AnimBlock -- Sequence Data
(
	frames, flags, fend, keys,
	
	fn Read bitStream tag =
	(
		ac = M3I_SeqData()
		ac.tag = tag
		ac.ablock = #()
		ac.ablock[tag.nData] = 0
		
		for i = 1 to tag.nData do
		(
			ab 			= M3I_AnimBlock()
			ab.frames 	= M3I_Ref.Read bitStream flags:#ulong
			ab.flags 		= ReadLong bitStream #unsigned
			ab.fend		= ReadLong bitStream #unsigned
			ab.keys 		= M3I_Ref.Read bitStream
			
			ac.ablock[i] = ab
		)
		
		return ac
	)
)

struct M3I_SeqCollection --Sequence Transformations Collection (STC)
(
	name, seqind, animid, animref, animoffs, sdata, cstart, cend,
	
	fn Read bitStream tag =
	(
		step = "Read Sequence Transformations"
		
		sc = #()
		sc[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			a = M3I_SeqCollection()

			a.name 		= M3I_Ref.Read bitStream
			SkipBytes bitStream 4
			a.seqind		= ReadShort bitStream #unsigned
			SkipBytes bitStream 2
			a.animid		= M3I_Ref.Read bitStream flags:#ulong
			a.animoffs	= M3I_Ref.Read bitStream flags:#animind
			SkipBytes bitStream 4
			-- sequence data array
			sda = #()
			sda[13] = 0
			for i = 1 to 13 do
			(
				s			= M3I_Ref.Read bitStream
				sda[i]	= s
			)
			a.sdata = sda
			
			sc[i] = a
		)
		
		return sc
	)
)

struct M3I_AnimLookup
(
	name, stcind,
	
	fn Read bitStream tag =
	(
		step = "Read Animation Lookup"
		
		al = #()
		al[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			local a = M3I_AnimLookup()
			a.name = M3I_Ref.Read bitStream
			a.stcind = M3I_Ref.Read bitStream flags:#ulong
			for k = 1 to a.stcind.count do
			(
				-- Must be positive for max arrays later
				a.stcind[k] += 1
			)
			
			al[i] = a
		)
		
		return al
	)
)

struct M3I_Bone
(
	name, parent, pos, transid, rotid, rot, scale, bindmat,
	
	fn Read bitStream tag =
	(
		step = "Read Bones"
		
		br = #()
		br[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			local b = M3I_Bone()
			SkipBytes bitStream 4
			b.name = M3I_Ref.Read bitStream
			--echo ("Bone_" + i as string + ": " + b.name)
			SkipBytes bitStream 4
			b.parent = ReadShort bitStream #signed + 1
			SkipBytes bitStream 2
			b.transid = M3I_AnimRef.Read bitStream
			b.pos = ReadVec bitStream #v3d
			SkipBytes bitStream 16
			b.rotid = M3I_AnimRef.Read bitStream
			b.rot = ReadVec bitStream #quat
			SkipBytes bitStream 28
			b.scale = ReadVec bitStream #v3d
			SkipBytes bitStream 36
			
			b.bindmat = (normalize b.rot) as matrix3
			b.bindmat.row4 = b.pos
				
			br[i] = b
		)
		echo ("nBones: "+br.count as string)
		
		return br
	)
)


struct M3I_Vertex
(
	pos, bw, bi, normal, uv,
	
	fn Read bitStream tag flags =
	(
		step = "Read Verts"
		
		local vbytes, vcount, bsize; --vertice/buffer size, many thanks to Volcore for discovering these flags and their meanings!
		vbytes = 0x20
		bsize = 0
		if ((bit.and flags 0x40000) != 0) then bsize += 0x04
		if ((bit.and flags 0x80000)  != 0) then bsize += 0x04
		if ((bit.and flags 0x100000) != 0) then bsize += 0x04
		vbytes += bsize
		
		vcount = tag.nData / vbytes
		local vread = #()
		vread[vcount] = 0
		for i=1 to vcount do
		(
			local v = M3I_Vertex()
			v.pos = ReadVec bitStream #v3d
			
			-- vertex bone weights
			v.bw = #()
			v.bw[4] = 0
			for j = 1 to 4 do 
			(
				local w = ReadByte bitStream #unsigned
				v.bw[j] = w / 255.0
			) 
			
			-- vertex bone index
			v.bi = #()
			v.bi[4] = 0
			for j = 1 to 4 do 
			(
				v.bi[j] = 1 + (ReadByte bitStream #unsigned) -- 1 + as maxscript arrays are 1 based
			)

			-- First read in compressed normals
			v.normal = #()
			v.normal[4] = 0
			for j = 1 to 4 do 
			(
				vnorm = ReadByte bitStream #unsigned
				vnorm = vnorm / 255.0
				vnorm = 2 * vnorm
				vnorm = vnorm - 1
				v.normal[j] = vnorm
			)
			
			-- ignore scale for now...seems insignificant
			local vn = [0,0,0] -- need point3 when building mesh
			--local w = v.normal[4]
			for j = 1 to 3 do (vn[j] = v.normal[j])
			v.normal = vn
			
			-- UV conversion
			uvp = [0,0,0] -- need point3 when setting tverts
			local uvm_sml = 2048.0
			for j=1 to 2 do 
			(
				local uv = ReadShort bitStream #signed
				uv = uv / uvm_sml
				--if uv > 1 then echo("Warning! Vert["+i as string+"].uv["+j as string+"]: "+uv as string+" is above 1!")
				if (j==2) and (flipuv_y) then (uv = 1 - uv) -- flip Y
				uvp[j] = uv
			)
			v.uv = uvp
			
			SkipBytes bitStream 0x04 -- tangent data
			
			-- Additional data due to flags
			SkipBytes bitStream bsize
			
			vread[i] = v
		)
		echo ("nVerts: " + vread.count as string)
		
		return vread
	)
)

struct M3I_DIV -- Model divisions
(
	faces, smeshes, bat, msec,
	
	fn Read bitStream =
	(
		step = "Read DIV"
		
		div = M3I_DIV()
		div.faces = M3I_Ref.Read bitStream flags:#ushort
		div.smeshes = M3I_Ref.Read bitStream
		div.bat = M3I_Ref.Read bitStream
		div.msec = M3I_Ref.Read bitStream

		echo ("nFaces: " + div.faces.count as string)
		echo ("nSubmeshes: " + div.smeshes.count as string)
		
		return div
	)
)

struct M3I_Submesh
(
	indverts, nverts, indfaces, nfaces, indbonelu, nbonelu,
	
	fn Read bitStream tag =
	(
		step = "Read Submeshes"
		
		smr = #()
		smr[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			sm = M3I_Submesh()
			case (head.fileID) of
			(
				#MD33:
				(
					SkipBytes bitStream 4
					sm.indverts = ReadShort bitStream #unsigned
					sm.nverts = ReadShort bitStream #unsigned
				)
				default:
				(
					SkipBytes bitStream 8
					sm.indverts = ReadLong bitStream #unsigned
					sm.nverts = ReadLong bitStream #unsigned
				)
			)

			sm.indfaces = ReadLong bitStream #unsigned
			sm.nfaces = ReadLong bitStream #unsigned
			SkipBytes bitStream 2
			sm.indbonelu = ReadShort bitStream #unsigned
			sm.nbonelu = ReadShort bitStream #unsigned
			SkipBytes bitStream 6
				
			smr[i] = sm
		)
		
		return smr
	)
)

struct M3I_Subref -- Submesh refrences
(
	subid, matid,
	
	fn Read bitStream tag =
	(
		step = "Read Submesh Material references"
		
		sr= #()
		sr[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			mat = M3I_Subref()
			SkipBytes bitStream 4
			mat.subid = ReadShort bitStream #unsigned + 1
			SkipBytes bitStream 4
			mat.matid = ReadShort bitStream #unsigned + 1
			SkipBytes bitStream 2
			
			sr[i] = mat
		)
		
		return sr
	)
)

struct M3I_MATM
(
	matind, matref,
	
	fn Read bitStream tag =
	(
		step = "Read MATM"
		
		mtm = #()
		mtm[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			matm = M3I_MATM()
			matm.matind = ReadLong bitStream #unsigned + 1
			matm.matref = ReadLong bitStream #unsigned + 1
			
			mtm[i] = matm
		)
		
		return mtm
	)
)

struct M3I_Layr --more work to be done at a later point
(
	name,
	
	fn Read bitStream = 
	(
		l = M3I_LAYR()
		
		SkipBytes bitStream 4
		ln = M3I_Ref.Read bitStream -- .dds file
		if (ln.count > 1) then l.name = M3I_Name ln "/" else l.name = ln
		SkipBytes bitStream 340
		
		return l
	)
)

struct M3I_Mat
(
	name, spec, layrs,

	fn Read_Layrs bitStream =
	(
		ml = #()
		-- Static 13 layers in Materials
		ml[13] = 0
		
		for j = 1 to 13 do
		(
			ml[j] = M3I_Ref.Read bitStream
		)
		
		return ml
	),
	fn Read bitStream tag =
	(
		step = "Read Materials"
		
		mr = #()
		mr[tag.nData] = 0
		for i = 1 to tag.nData do
		(
			m = M3I_Mat()
			m.name = M3I_Ref.Read bitStream
			--echo ("Mat_" + i as string + ": " + m.name)
			SkipBytes bitStream 0x14
			m.spec = ReadFloat bitStream
			SkipBytes bitStream 0x10
			m.layrs = M3I_Mat.Read_Layrs bitStream
			SkipBytes bitStream 60
			
			mr[i] = m
		)
		
		echo ("nMaterials: " + mr.count as string)	
		return mr
	),
	fn MapChk tname =
	(
		if (tname.count > 1) then
		(
			mcount = mapPaths.count()
			for i = 1 to mcount do
			(
				local tp = mapPaths.get i + "\\"
				
				tex = tp + tname
				if (doesFileExist(tex)) then
				(
					return tex
				)
				else if (i == mapPaths.count()) then
				(
					return 0
				)
			)
		)
		else
		(
			return 1
		)
	),
	fn Check =
	(
		step = "Check Materials"
		
		-- Material bitmap check
		local matlu = mread.DIV.bat
		local ss = stringstream "", fmt = "%\n"
		local ss_f = false
		for i = 1 to matlu.count do
		(
			-- find material referenced in lookup table
			mat_chk = matlu[i].matID
			mid = mread.matm[mat_chk].matref
			m = mread.mat[mid]
			
			for j = 1 to 13 do
			(
				-- Not sure of a more effective way to do this...
				if (j == 1 or j == 3 or j == 4 or j == 6 or j == 10) then
				(
					-- Check if file exists
					local map = m.layrs[j].name
					t = M3I_Mat.MapChk map
					-- If it doesn't, add it to error output for later
					if (t == 0) then 
					(
						-- prevent duplicates being added to list
						local strchk = ss as string
						local strmapchk = "*" + (map as string) + "*"
						if (matchPattern strchk pattern:strmapchk ignoreCase:true != true) then
						(
							format fmt map to:ss
						)
						
						-- Trip error switch
						ss_f = true
					)
				)
			)
		)
		
		if (ss_f) then
		(
			-- Error text
			textErr = "The following textures were not found in your 3ds maps directories:\n"
			textErr += ss
			echo textErr
			textErr += "These textures will not be applied to the model. Continue?"
			local t_scrTime = TimeStamp()
			if (queryBox textErr beep:false != true) then
			(
				-- Flip script exit, disable all further script activity
				echo "ERROR# Textures not found"
				scrExit = true
				doCreateBones = doTransformBones = doCreateSkin = doCreateMesh = doCreateMats = false
			)
			else
			(
				-- Correct for time spent dealing with queryBox
				local t_scrTime = TimeStamp() - t_scrTime
				scrStart += t_scrTime
			)
		)
	)
)

struct M3I_Animoffs 
(
	aind, sdind,
	
	fn Read bitStream =
	(
		a = M3I_Animoffs()
		a.aind	= ReadShort bitStream #unsigned + 1
		a.sdind 	= ReadShort bitStream #unsigned + 1
		
		return a
	)
)

fn M3I_Read_Data bitStream tag flags = --Reading U32_, I32_, U8__, etc.
(
	local u = #()
	u[tag.nData] = 0
	for i = 1 to tag.nData do
	(
		local a
		case (flags) of
		(
			#animind: 
			(
				a	= M3I_Animoffs.Read bitStream
			)
			#ubyte: a = ReadByte bitStream #unsigned
			#ushort: a = ReadShort bitStream #unsigned
			#ulong: a = ReadLong bitStream #unsigned 
		)
		
		u[i] = a
	)
	
	return u
)

fn ReadTagData bitStream tagInd flags =
(
	local bm = ftell(bitStream) -- bookmark first
	local d = #()
	th = tread[tagInd]

	if ((fseek bitStream th.ofsData #seek_set) and (th.nData > 0)) then
	(
		case (th.dataID) of
        (
			#MODL: d = M3I_Read_MODL bitStream th
			#SEQS: d = M3I_AnimData.Read bitStream th
			#STC_: d = M3I_SeqCollection.Read bitStream th
			#STG_: d = M3I_AnimLookup.Read bitStream th
			#SD4Q: d = M3I_AnimBlock.Read bitStream th
			#SD3V: d = M3I_AnimBlock.Read bitStream th
			#SDR3: d = M3I_AnimBlock.Read bitStream th
			#U8__ : d = M3I_Vertex.Read bitStream th flags
			#U16_: d = M3I_Read_Data bitStream th flags
			#U32_: d = M3I_Read_Data bitStream th flags
			#I32_: d = M3I_Read_Data bitStream th flags
			#VEC3: d = ReadVec bitStream #v3d array:th.nData
			#QUAT: d = ReadVec bitStream #quat array:th.nData
			#DIV_: d = M3I_DIV.Read bitStream
			#REGN: d = M3I_Submesh.Read bitStream th
			#BAT_: d = M3I_Subref.Read bitStream th
			#MATM: d = M3I_MATM.Read bitStream th
			#MAT_: d = M3I_Mat.Read bitStream th
			#LAYR: d = M3I_Layr.Read bitStream
			#BONE: d = M3I_Bone.Read bitStream th
			#CHAR: d = ReadFixedString bitStream th.nData
			#IREF: d = ReadVec bitStream #matrix array:th.nData
			default: d = M3I_Read_Data bitStream th #ubyte
		)
	)
	else
		"Read Tag failed!"
	
	fseek bitStream bm #seek_set -- return to bookmark
	return d
)

-- ***************************
--  MAX SCENE FUNCTIONS
-- ***************************
-- Scene data funcs
fn Set_Scene_Data obj &ind val  =
(
	setAppData obj ind (val as string) 
	ind+=1
)

fn Get_Scene_Data obj &ind  =
(
	ind+=1 
	return getAppData obj (ind-1)
)

fn Set_Scene_Section obj &ind val =
(
 	Set_Scene_Data obj &ind val.name
	Set_Scene_Data obj &ind val.cstart 
	Set_Scene_Data obj &ind val.cend
)

fn Get_Scene_Section obj &ind  =
(
	ss = M3I_AnimData()
	ss.name 	= Get_Scene_Data obj &ind
	ss.cstart 	= Get_Scene_Data obj &ind
	ss.cend 		= Get_Scene_Data obj &ind 
	return ss
)

fn M3I_Store_Anims =
(
	step = "Storing Animation Data"
	echo "Storing Animation Data in Scene..."

	-- Initiate Scene Object
	if ($_M3I_Anim_Data != undefined) then delete($_M3I_Anim_Data)
	-- Create scene object to house animation information
	local ad = point size:0.001 pos:[0,0,0]
	ad.name = "_M3I_Anim_Data"
	ad.renderable = off
	ad.isHidden = true
	ad.wirecolor = color 8 8 136
	
	local t = 1
	local astart = 2 -- leave keyframe 0 and 1 as bindposes
	local buffer = 2000 -- buffer between animations
	al = mread.STG
	sq = mread.SEQS
	Set_Scene_Data ad &t aread.count
	for i = 1 to al.count do
	(
		a	= sq[i]
		alu = al[i]
		
		for k = 1 to alu.stcind.count do
		(
			stcid = alu.stcind[k]
			st = aread[stcid]
			st.name = (st.name+"["+(i-1) as string+"]")
			st.cstart = astart
			st.cend	= astart + a.maxframes
			
			astart = st.cend + (M3I_Convert_Time buffer)
			
			Set_Scene_Section ad &t st
		)
	)
	frameRate = useFPS
)

-- BONE FUNCTIONS
-- from the maxscript reference
fn setNodeWorldRotation theNode theRot = 
( 
	in coordsys (transmatrix theNode.transform.pos) 
 		theNode.rotation = theRot
) 

fn M3I_Bone_Depth b =
(
	-- For organising bones based on depth
	if b.parent == 0 then 0
	else return ( 1 + M3I_Bone_Depth mread.bones[b.parent] )
)

fn M3I_Get_Bone_Depth bones =
(
	-- sort bones by depth
	bd = #()
	bd[bones.count] = 0
	for i=1 to bones.count do
	(
		bonerec = [(M3I_Bone_Depth bones[i]), i]
		bd[i] = bonerec
	)
	fn compfn a b = ( if a.x<b.x then return 1; else if a.x>b.x then return -1; else return 0; )
	qsort bd compfn
	
	return bd
)

fn M3I_Bones_Bindpose bones mbones =
(
	local bd = M3I_Get_Bone_Depth mbones
	
	for i=1 to bones.count do
	(
		local h = bd[i].y
		b = mbones[h]
		cb = bones[h]
		
		in coordsys parent
		(
			cb.scale = b.scale
			cb.rotation = b.rot
			cb.position = b.pos
		)
	)
)

fn M3I_Create_Bones =
(
	step = "Create Bones"
	echo "Creating bones..."
	cbones = #()
	
	mb = mread.bones
	for i = 1 to mb.count do
	(
		b = mb[i]
		--echo ("Creating bone_" + i as string + ": " + b.name)
		
		cb = BoneSys.createBone [0,0,0] [0,0,0] [0,0,0.1]
		cb.name 		= b.name
		cb.width 		= bonesize
		cb.height		= bonesize
		cb.showLinks	= true

		append cbones cb
	)
	max views redraw
	
	echo "Setting up bone hierarchy"
	for i=1 to mb.count do
	(
		b = mb[i]
		if b.parent!=0 then
		(
			-- echo ("Setting parent of " + i as string + " to " + (bones_read[i].par+1) as string)
			cbones[i].parent = cbones[b.parent]
		)
	)
	
	echo "Set up bones bindpose"
	M3I_Bones_Bindpose cbones mread.bones

	max views redraw
)

fn M3I_Animate_Bone boneInd stcInd animInd =
(
	-- get transformation array indices
	abInd = stcInd.animoffs[animInd].aind
	seqInd = stcInd.animoffs[animInd].sdind
	seqData = stcInd.sdata[seqInd]
	ablock = seqData.ablock[abInd]
	
	-- set bones/created bones
	b = mread.bones[boneInd]
	cb = cbones[boneInd]
	
	-- 2 extra frames in the bindpose either side of the animation
	-- prevents crazy translation between animations

	for i = 1 to ablock.frames.count do
	(
		local t, key
		t = stcInd.cstart + (M3I_Convert_Time ablock.frames[i])
		key = ablock.keys[i]
		
		with animate on
		(	
			at time t
			(
				case (seqData.tag.dataID) of
				(
					#SD3V: in coordsys parent cb.pos = key
					#SD4Q:
					(
						local par = cb.parent
						if par != undefined then q = par.rotation * key else q = key
						setNodeWorldRotation cb q
						--in coordsys parent cb.rotation = key
					)
				) -- end of case
			)
		) -- end animation handling
	) -- end of for loop
)

fn M3I_Setup_Anim boneIndex refID =
(
	-- Gotta clean up animation code at some point...very messy :(
	local stg = mread.STG
	for i = 1 to stg.count do
	(
		-- Find STC's for animation
		stgInd = stg[i]
		for k = 1 to stgInd.stcind.count do
		(
			stcInd = stgInd.stcind[k]
			-- 1. Find refid in animid list
			stc = aread[stcInd]
			animID = stc.animid
			local IDfound = 0
			
			for i = 1 to animID.count do
			(
				if (animID[i] == refID) then
				(
					IDfound = i
					-- more efficient for breaking for loop, 'break()' lags maxscript
					i = animID.count + 1
				)
			)
			
			-- 2. If refid found -> transform bone with corresponding animation data
			-- if not found -> bone refid references no animation data
			if (IDfound != 0) then
			(
				M3I_Animate_Bone boneIndex stc IDfound
			)
		)
	)
)

fn M3I_Animate_Bones =
(
	---------------------- animation
	-- sort bones by depth
	mbones = mread.bones
	local bd = M3I_Get_Bone_Depth mbones
	
	-- bindpose before and after animations
	-- prevents funny translations between animations
	echo "Setting up bindposes between animations..."
	stg = mread.STG
	seqs = mread.SEQS
	for i = 1 to seqs.count do
	(
		seqInd = seqs[i]
		stgInd = stg[i]
		
		for j = 1 to stgInd.stcind.count do
		(
			stcInd = stgInd.stcind[j]
			stc = aread[stcInd]
			aprev = stc.cstart - 1
			anext = stc.cstart + seqInd.maxframes + 1
			poseFrames = #(aprev, anext)
			
			for k = 1 to 2 do
			(
				with animate on
				(
					at time poseFrames[k]
					(
						M3I_Bones_Bindpose cbones mread.bones
					)
				)
			)
		)
	)
	
	echo "Transforming bones..."
	step = "Bone transforms"
	for k=1 to mbones.count do
	(
		local bind = bd[k].y
		--echo ("Doing bone " + i as string + " (depth: " + bd[k].x as string + ")" )
		--echo ("Transforming bone " + i as string)
		b = mread.bones[bind]

		-- translation
		M3I_Setup_Anim bind b.transid.animid
		-- rotation
		M3I_Setup_Anim bind b.rotid.animid
	)
	
	echo "Bone animation done\n"
)

fn M3I_Assign_Maps map texname =
(
	local t = M3I_Mat.MapChk texname
	if (isKindOf t string) then
	(
		bt = BitMapTexture()
		bt.filename = t
		map = bt
	)
	
	return map
)

fn M3I_Create_Mats =
(
	step = "Create Materials"
	
	echo "Creating Materials..."
	
	matlu = mread.DIV.bat
	cm = multimaterial numsubs:matlu.count
	
	for i = 1 to matlu.count do
	(
		-- find material referenced in lookup table
		mat_chk = matlu[i].matID
		mid = mread.matm[mat_chk].matref
		m = mread.mat[mid]
		
		local sm = standardMaterial name:(m.name)
		cm[i] = sm
		
		sm.specularLevel = m.spec
		sm.diffuseMap = M3I_Assign_Maps sm.diffuseMap m.layrs[1].name
		sm.specularMap = M3I_Assign_Maps sm.specularMap m.layrs[3].name
		sm.selfIllumMap = M3I_Assign_Maps sm.selfIllumMap m.layrs[4].name
		if (sm.selfIllumMap != undefined) then sm.useSelfIllumColor = true
		sm.ReflectionMap = M3I_Assign_MAps sm.ReflectionMap m.layrs[6].name
		sm.bumpMap = M3I_Assign_Maps sm.bumpMap m.layrs[10].name
		if (sm.bumpMap != undefined) then sm.bumpMapAmount = 100
		
		-- turn on maps in viewports
		showTextureMap sm on
		redrawViews()
	)
	
	return cm
)

fn M3I_Create_Mesh =
(
	step = "Create Mesh"

	echo "Creating mesh..."

	v = #()
	f = #()
	t = #()
	mtl = #()

	vr = mread.verts 
	fr = mread.DIV.faces

	echo "Gathering vertices..."
	-- build vertices and texcoords
	for i = 1 to vr.count do
	(
		append v vr[i].pos
		append t vr[i].uv
	)

	echo "Gathering submeshes..."
	smr = mread.DIV.smeshes
	matlu = mread.DIV.bat
	for s = 1 to matlu.count do
	(
		-- Building the Submeshes
		-- build faces by submesh
		-- (each submesh will be a material id)
		mat = matlu[s]
		sm = smr[mat.subid]
		local up = sm.nfaces
		local find = sm.indfaces
		local vind = sm.indverts
		--echo ("sm_"+s as string+".nfaces: "+up as string +" indfaces:" + sm.indfaces as string)

		if ((mod (up) 3) != 0.0 ) then echo "#ERROR sm.tris not a multiple of 3!"
		--else echo "#INFO sm.tris check passed!"
		
		--echo ("Creating Submesh_" + s as string)
		for i = 1 to (up) by 3 do
		(
			local face = [0,0,0]
			for j = 1 to 3 do
			(
				local k = find + i + j - 1
				case (head.fileID) of
				(
					#MD33: face[j] = fr[k] + 1
					default: face[j] = fr[k] + vind + 1
				)
			)
			
			append f face
			append mtl s
		)
	)

	echo "Creating mesh object..."
	cmesh = mesh vertices:v faces:f materialIDs:mtl tverts:t name:(mread.name)

	select cmesh
	max modify mode
	
	-- if importing mats, apply to mesh
	if doCreateMats then
	(
		cmat = M3I_Create_Mats()
		
		echo "Applying materials..."
		cmesh.material = cmat
		cmesh.material.showInViewport = true
		setMeditMaterial 1 cmat
	)
	
	update cmesh

	-- tvert faces
	if vr.count != 0 do
	(
		echo "Building texture vertices..."
		step = "Build tv's"
		try
		(
			--Set texcoord faces 
			buildTVFaces cmesh false
			for i = 1 to f.count do
			(
				setTVFace cmesh i f[i]
			)
		)
		catch
		(
			echo (getCurrentException())
			throw ()
		)
	)
	update cmesh
	
	-- if doing skin, set normals for bindpose instead
	if (doVertexNormals == true and doCreateSkin != true) then
	(
		echo "Setting vertex normals..."
		addmodifier cmesh (edit_normals name:"vnorms")
		en = cmesh.modifiers[#vnorms]
		modPanel.setCurrentObject en

		en.displaylength = 0.05
		
		-- speeds up processing
		en_SetExplicit = en.SetNormalExplicit
		en_SetNormal = en.SetNormal
		en_ConvVert	= en.ConvertVertexSelection
		en_SetNormalID = en.SetNormalID
		
		-- Method: 1
		for i = 1 to vr.count do
		(
			my_vert = #{i}
			my_norm = #{}
			en_ConvVert &my_vert &my_norm
			en.SetSelection my_norm
			en.unify selection:my_norm
			en.MakeExplicit selection:my_norm
			en.move vr[i].normal
		)
		
		/*
		-- Method 2:
		-- way too slow...but sets normal ID to vertex ID's
		for i = 1 to en.GetNumFaces() do
		(
			for j = 1 to 3 do
			(
				vind = en.getVertexID i j
				en_SetNormalID i j vind
				en_SetExplicit vind
				en_SetNormal vind vr[vind].normal
			)
		)
		en.RebuildNormals()
		*/

		-- collapse edit_normal modifier
		collapseStack cmesh
	)
	
	-- Bounding Sphere
	echo "Creating bounding sphere..."
	local bsphere = (mread.bndSphere)
	local pos = (bsphere.vmin + bsphere.vmax) / 2 -- get centre
	s = Sphere position:pos radius:bsphere.rad
	-- Hide it for now...
	s.isHidden = true
	
	step = "Create Mesh Done"
	echo "Mesh done\n"
)

fn M3I_Apply_Skin =
(
	---------------------- skinning
	step = "Create Skin"
	echo "Applying skin to mesh..."
	
	m = cmesh
	select m
	
	max modify mode
	
	echo "Adjusting vertices..."
	-- 1. for each vertex, calculate bindpose stuff, set position
	vread = mread.verts
	for j=1 to vread.count do
	(
		local v = vread[j]
		--echo (j as string + ": " + v.bi[1] as string + ", " + v.bi[2] as string + ", " + v.bi[3] as string + ", " + v.bi[4] as string \
		--	+ " --- (" + v.bw[1] as string + "," + v.bw[2] as string + "," + v.bw[3] as string + "," + v.bw[4] as string + ")")
		local boneInds = #()
		local boneWeights = #()
		local finalPos = [0,0,0]
		local finalNorm = [0,0,0]
		
		for h = 1 to 4 do
		(
			if v.bw[h]>0 then 
			( 
				k = v.bi[h]
				vweight = v.bw[h]
				smeshes = mread.DIV.smeshes
				local b
				for s = 1 to smeshes.count do
				(
					smesh = smeshes[s]
					local minv = smesh.indverts
					local maxv = minv + smesh.nverts + 1
					if j > minv and j < maxv then
					(
						bind = smesh.indbonelu + k
						b = mread.blist[bind] + 1
					)
				)
				
				at time 0
				(
					local pos
					local norm
					cb = cbones[b]
					
					local iref = mread.IREF[b]
					local bscale = cb.scale
					local brot = (inverse cb.rotation)
					local bpos = cb.pos
					
					-- recalculate vert position
					pos = v.pos * iref
					pos = pos * bscale
					pos = pos * brot
					finalPos += (bpos + pos) * vweight
					
					-- recalculate vert normal
					norm = v.normal * iref
					norm = norm * bscale
					norm = norm * brot
					finalNorm += (bpos + norm) * vweight
				)

				append boneInds b
				append boneWeights v.bw[h]
			)
		)
		
		-- set vertex bone indices and weights
		v.bi = boneInds
		v.bw = boneWeights
		
		--echo ("vert["+j as string+"].bi: " + v.bi as string + " v.bw: " + vbw as string)
		
		-- set vertex to bindpose position
		v.pos = finalPos
		
		-- set normal
		v.normal = finalNorm
		
		-- move vertex to bindpose position
		meshop.setVert m j v.pos
	)
	
	-- #vnorms
	-- 2. Set normals in bindpose
	if (doVertexNormals != false) then
	(
		echo "Setting vertex normals..."
		addmodifier m (edit_normals name:"vnorms")
		en = m.modifiers[#vnorms]
		modPanel.setCurrentObject en
		en.displaylength = 0.05
		
		for j = 1 to vread.count do
		(
			local vert = vread[j]
			local my_vert = #{j}
			local my_norm = #{}
			
			en.ConvertVertexSelection &my_vert &my_norm
			en.SetSelection my_norm
			en.unify selection:my_norm
			en.MakeExplicit selection:my_norm
			en.move vert.normal
		)
	)
	
	-- collapse edit_normals modifier
	collapseStack m
	
	-- #Skin
	addModifier m (Skin name:"Skin")
	sk = m.modifiers[#Skin]
	modPanel.setCurrentObject sk

	-- 3. add all bones to the skin
	echo "Adding bones to skin..."
	for j=1 to cbones.count do
	(
		skinOps.addBone sk cbones[j] 0
	)
	
	update m
	max views redraw
	
	echo "Setting vertex weights..."
	disableSceneRedraw() -- speeds up weight assignment
	-- 4. Set vertex weights
	for j = 1 to vread.count do
	(
		local vert = vread[j]
		skinOps.ReplaceVertexWeights sk j vert.bi vert.bw
	)
	enableSceneRedraw()
	
	update m
	redrawViews()
	step = "Skinning complete"
	echo "Skinning done\n"
)

fn M3I_Zoom_Model =
(
	step = "Zoom Viewports to Model"
	
	viewport.ZoomToBounds true cmesh.min cmesh.max
)
-- ********
--  MAIN
-- ********
-- Import
fn M3I_Main file =
(	
	-- Read Model Data
	echo "\n====== Import Starting ======\n"
	bstream = M3I_Open file "rb"
	M3I_Read_Start bstream
	M3I_Close(bstream)
	if (doCreateMats and doCreateMesh) then M3I_Mat.Check()
	
	-- Max Scene Objects
	if (scrExit != true) then echo "Creating Scene objects..."
	-- Bones
	if doCreateBones then
	(
		Try 
		(
			M3I_Create_Bones()
		)
		Catch
		(
			echo "ERROR# Bone import failed!"
			echo ("****"+getCurrentException()+"****")
			doCreateBones = false
			doCreateSkin = false
			doTransformBones = false
		)
	)
	-- Bone Animation
	if doCreateBones and doTransformBones then
	(
		Try
		(
			M3I_Store_Anims()
			M3I_Animate_Bones()
			M3I_AnimUI()
		)
		Catch
		(
			echo "ERROR# Bone animation failed!"
			echo ("****"+getCurrentException()+"****")
			doTransformBones = false
		)
	)
	-- Mesh
	if doCreateMesh then
	(
		Try
		(
			M3I_Create_Mesh()
			M3I_Zoom_Model()
		)
		Catch
		(
			echo "ERROR# Mesh import failed!"
			echo ("****"+getCurrentException()+"****")
			doCreateMesh = false
			doCreateSkin = false
			doVertNorms = false
			if isSceneRedrawDisabled() then enableSceneRedraw()
		)
	)
	-- Skin
	if doCreateBones and doCreateSkin and doCreateMesh then
	(
		Try 
		(
			M3I_Apply_Skin()
		)
		Catch
		(
			echo "ERROR# Skin creation failed!"
			echo ("****"+getCurrentException()+"****")
			doCreateSkin = false
			-- Fix viewport rendering if skinning failed during vertex weighting
			if isSceneRedrawDisabled() then enableSceneRedraw()
		)
	)
	
	-- Check if script exit has been flipped
	if (scrExit) then
	(
		echo "======= Import Aborted =======\n"
	)
	else
	(
		-- Calculate Script Execution time
		scrEnd = TimeStamp()
		format "Import took % seconds\n" ((scrEnd - scrStart) / 1000.0)
		echo "====== Import Successful ======\n"
	)

	setCommandPanelTaskMode #utility
	max views redraw
	M3I_Reset_Globals()
)

-- *********************************
--  USER INTERFACE FUNCTIONS
-- *********************************
-- Global UI Funcs
fn uiOpenFile ftypes &fname =
(
	local ret = getOpenFileName types:ftypes filename:fname
	if ret != undefined then fname = ret
)

utility m3imp "M3 - Import"
(
	group "Model Filename"
	(
		button bOpenFile "Open..."
		edittext tFileName
	)
	
	group "Settings"
	(
		checkBox chkImportMesh "Import Mesh" checked:(doCreateMesh) tooltip:"Imports Model Geometry"
		checkBox chkImportNorms "Import Vertex Normals" checked:(doVertexNormals)
		checkBox chkImportMats "Import Materials" checked:(doCreateMats) tooltip:"Imports Model Materials"
		checkBox chkImportSkin	"Create Skin" checked:(doCreateSkin) enabled:(doCreateBones and doCreateMesh) tooltip:"Deforms mesh through bones"
		checkBox chkImportBones "Import Bones" checked:(doCreateBones) tooltip:"Imports Bones from Model (buggy)"
		checkBox chkTransformBones "Animate Bones" checked:(doTransformBones) enabled:(doCreateBones) tooltip:"Animates model bones"
		spinner spnFPS "FPS:" range:[1,1000,1000] enabled:(doTransformBones) type:#integer
	)
	
	button bImport "Import" height:35 width:100
	
	on m3imp open do
	(
		if (doCreateBones != true) then
		(
			chkImportSkin.checked = false
			chkTransformBones.checked = false
		)
		if (chkTransformBones.checked != true) then
		(
			spnFPS.enabled = false
		)
		if (doCreateMesh != true) then
		(
			chkImportSkin.checked = false
		)
	)
	on chkImportMesh changed state do
	(
		if (state == on) then
		(
			if (chkImportBones.checked != false) then
			(
				chkImportSkin.checked = true
				chkImportSkin.enabled = true
			)
			chkImportNorms.checked = true
			chkImportNorms.enabled = true
			chkImportMats.checked = true
			chkImportMats.enabled = true
		)
		else
		(
			chkImportNorms.checked = false
			chkImportNorms.enabled = false
			chkImportSkin.checked = false
			chkImportSkin.enabled = false
			chkImportMats.checked = false
			chkImportMats.enabled = false			
		)
	)
	on chkImportBones changed state do
	(
		if (state == on) then
		(
			chkTransformBones.checked = true
			chkTransformBones.enabled	= true
			if (chkImportMesh.checked != false) then
			(
				chkImportSkin.checked = true
				chkImportSkin.enabled = true
			)
			spnFPS.enabled = true			
		)
		else
		(
			chkTransformBones.checked = false 	
			chkTransformBones.enabled = false
			chkImportSkin.checked = false
			chkImportSkin.enabled = false
			spnFPS.enabled = false
		)
	)
	on chkTransformBones changed state do
	(
		if (state == on) then
		(
			spnFPS.enabled = true
		)
		else
		(
			spnFPS.enabled = false			
		)
	)
	on bOpenFile pressed do
	(
		uiOpenFile "M3 model (*.m3)|*.m3|All Files|*.*|" &tFileName.text
	)
	on bImport pressed do
	(
		local fname = tFilename.text
		if doesFileExist(fname) then
		(
			doCreateMesh = chkImportMesh.checked
			doVertexNormals = chkImportNorms.checked
			doCreateSkin = chkImportSkin.checked
			doCreateMats = chkImportMats.checked
			doCreateBones = chkImportBones.checked
			doTransformBones = chkTransformBones.checked
			useFPS = spnFPS.value
			M3I_Main(fname)
			if (scrExit != true) then m3imp.open = false
		)
	)
)

utility animUI "M3 - Sequences"
(
	local animList = #("None")
	local aobj = $_M3I_Anim_Data
	local adata = #()
	
	dropdownlist	dlAnim "Animation:" items:#("None")
	edittext		edtStart "Time Start:" enabled:false
	edittext		edtEnd "Time End: " enabled:false
	
	fn Selected sel =
	(
		local ad = adata[sel-1]
		animationRange = interval (ad.cstart as integer) (ad.cend as integer)
		sliderTime = (ad.cstart as integer)
		edtStart.text = ad.cstart
		edtEnd.text = ad.cend
	)
	
	fn NoneSelected =
	(
		if (adata.count > 0) then 
		(
			animationRange = interval 0 (adata[adata.count].cend as integer)
			edtEnd.text = adata[adata.count].cend
		)
		else 
		(
			animationRange = interval 0 100
			edtEnd.text = "100"
		)
		edtStart.text = "0"
		edtStart.enabled = false
		edtEnd.enabled = false
	)
	
	fn List_Repop  =
	(
		animList = dlAnim.items
		animList[adata.count+1] = 0
		animList[1] = "None"
		for i = 1 to adata.count do
		(
			--echo (anim_data[i].name)
			animList[i+1] = adata[i].name
		)
		
		rsel = random 1 adata.count
		
		dlAnim.items = animList
		dlAnim.selection = rsel + 1
		Selected dlAnim.selection
	)
	
	fn List_Pop =
	(
		if (aobj != undefined) do
		(	
			--reset vars
			adata = #()
			
			local t = 1
			local animCount = (Get_Scene_Data aobj &t) as integer
			adata[animCount] = 0
			for h = 1 to animCount do
			(
				adata[h] = Get_Scene_Section aobj &t
			)
			
			List_Repop()
		)			
	)
	
	on animUI open do
	(
		List_Pop()
	)
	on dlAnim selected i do
	(
		if (adata.count > 0) then
		(
			if (i == 1) then
			(
				NoneSelected()
			)
			else
			(
				Selected i
			)
		)
	)
)

fn M3I_AnimUI =
(
	-- Reset
	if (animUI != undefined) do closeUtility animUI
	openUtility animUI
)