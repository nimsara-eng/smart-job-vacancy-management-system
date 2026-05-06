import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import path from 'path'
import fs from 'fs'

export default defineConfig({
  plugins: [
    vue(),
    {
      // Serve Job-Vacancy-System/data/ at the /data route in dev
      name: 'serve-root-data',
      configureServer(server) {
        server.middlewares.use('/data', (req, res, next) => {
          const fileName = (req.url ?? '/').replace(/^\//, '')
          const filePath = path.resolve(__dirname, '../data', fileName)

          if (fileName && fs.existsSync(filePath) && fs.statSync(filePath).isFile()) {
            res.setHeader('Content-Type', 'application/json; charset=utf-8')
            res.setHeader('Cache-Control', 'no-store') // always read latest from disk
            fs.createReadStream(filePath).pipe(res as any)
          } else {
            next()
          }
        })
      },
    },
  ],
})