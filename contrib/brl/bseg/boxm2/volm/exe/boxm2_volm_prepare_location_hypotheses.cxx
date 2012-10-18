//:
// \file
// \brief an executable to prepare location hypotheses for volumetric matcher
// \author Ozge C. Ozcanli
// \date Oct 11, 2012

#include <vul/vul_arg.h>

#include <boxm2/volm/boxm2_volm_locations.h>
#include <boxm2/boxm2_scene.h>
#include <vil/vil_image_view.h>
#include <vil/vil_save.h>
#include <vil/vil_load.h>


int main(int argc,  char** argv)
{
  vul_arg<vcl_string> scene_file("-scene", "scene xml filename", "");
  vul_arg<vcl_string> dem_file("-dem", "dem name, assume there is a tfw file in the same folder ", "");
  vul_arg<vcl_string> out_file("-out", "prefix for names of output binary files", "");
  vul_arg<unsigned> int_i("-int_i", "interval in +x direction (East) in pixels in the output tiles", (unsigned)10);
  vul_arg<unsigned> int_j("-int_j", "interval in +y direction (North) in pixels in the output tiles", (unsigned)10);
  vul_arg<float> alt("-altitude", "altitude (in meters) from above ground to generate the elevation of each hyptohesis, default is 1.6", 1.6f);
  vul_arg_parse(argc, argv);

  vcl_cout << "argc: " << argc << vcl_endl;
  if (scene_file().compare("") == 0 || dem_file().compare("") == 0 || out_file().compare("") == 0) {
    vcl_cerr << "EXE_ARGUMENT_ERROR!\n";
    vul_arg_display_usage_and_exit();
    return 0;
  }
  boxm2_scene_sptr scene = new boxm2_scene(scene_file());
  vpgl_lvcs_sptr lvcs = new vpgl_lvcs(scene->lvcs());

  vcl_vector<vil_image_view<float> > dems;
  vcl_vector<vpgl_geo_camera* > cams;
  vil_image_resource_sptr dem_res = vil_load_image_resource(dem_file().c_str());
  vil_image_view<float> dem = dem_res->get_view();
  vpgl_geo_camera* geocam = 0;
  vpgl_geo_camera::init_geo_camera(dem_res, lvcs, geocam);

  dems.push_back(dem);
  cams.push_back(geocam);

  vcl_cout << "generating hypotheses for scene: " << scene_file() << " using " << dem_file() << vcl_endl;

  //: just generate dummy output
  vcl_vector<volm_tile> tiles = volm_tile::generate_p1_tiles();

  for (unsigned i = 0; i < tiles.size(); i++) {
    vcl_string out_name = out_file() + "_volm_" + tiles[i].get_string() + ".kml";
    tiles[i].write_kml(out_name, 1000);
  }

  for (unsigned i = 0; i < tiles.size(); i++) {
    boxm2_volm_loc_hypotheses h(lvcs, tiles[i], int_i(), int_j(), alt(), dems, cams);
    vcl_cout << "constructed: " << h.locs_.size() << " hypotheses for tile: " << tiles[i].get_string() << vcl_endl;
    //vcl_cout <<

    vil_image_view<unsigned int> out(3601, 3601);

    out.fill(volm_io::UNEVALUATED);
    vcl_vector<float> dummy_scores(h.locs_.size(), 1.0f);
    h.generate_output_tile(dummy_scores, 5*int_i()/12, 5*int_j()/12, 0.5f, out);

    vcl_string out_name = out_file() + "_volm_" + tiles[i].get_string() + ".tif";
    vil_save(out, out_name.c_str());
  }

  vcl_cout << "returning SUCCESS!\n";
  return volm_io::SUCCESS;
}