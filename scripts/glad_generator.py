import os
import shutil
import subprocess

from pathlib import Path
from yaml import load, Loader

if __name__ == "__main__":
    cur_dir = Path(os.getcwd())

    file_path = __file__
    parent_dir = Path(file_path).parent
    project_dir = parent_dir.parent
    tmp_dir = parent_dir.joinpath("__tmp")
    
    include_dir = project_dir.joinpath("external/include")

    glad_src_dir = project_dir.joinpath("src/glad.c")
    glad_include_dir = include_dir.joinpath("glad/glad.h")
    khr_include_dir = include_dir.joinpath("KHR/khrplatform.h")

    shutil.rmtree(tmp_dir, ignore_errors=True)
    os.makedirs(tmp_dir)
    
    with open(Path(parent_dir).joinpath("project_config.yaml")) as f:
        glad = load(f, Loader=Loader)["glad"]
        API = f"gl={glad['version']}"
        extensions = ",".join(glad["extensions"])
        spec = glad["spec"]
        profile = glad["profile"]

        cmd = f"glad --generator c --out-path {tmp_dir} --profile {profile} --api {API} --spec {spec} --extensions {extensions}"
        subprocess.run(cmd)

        files = os.listdir(tmp_dir)

        if len(files) > 0:
            glad_src_dir.unlink(missing_ok=True)
            khr_include_dir.unlink(missing_ok=True)
            glad_include_dir.unlink(missing_ok=True)
            shutil.copyfile(tmp_dir.joinpath("src/glad.c"), glad_src_dir)
            shutil.copyfile(tmp_dir.joinpath("include/glad/glad.h"), glad_include_dir)
            shutil.copyfile(tmp_dir.joinpath("include/KHR/khrplatform.h"), khr_include_dir)