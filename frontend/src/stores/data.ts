import { defineStore } from 'pinia'
import type { Job, User, Application } from '../types'

// ─── Helpers: read / write JSON files via fetch ─────────────────────────────
async function loadJSON<T>(path: string): Promise<T> {
  const res = await fetch(path)
  if (!res.ok) throw new Error(`Failed to load ${path}`)
  return res.json()
}

export const useDataStore = defineStore('data', {
  state: () => ({
    jobs: [] as Job[],
    users: [] as User[],
    applications: [] as Application[],
    loading: false,
    error: null as string | null
  }),
  actions: {
    // ── Persistence Helper ──────────────────────────────────────────────────
    // Saves current state to the browser's local storage to survive refreshes
    saveData() {
      localStorage.setItem('sjvms_jobs', JSON.stringify(this.jobs))
      localStorage.setItem('sjvms_users', JSON.stringify(this.users))
      localStorage.setItem('sjvms_applications', JSON.stringify(this.applications))
    },

    // ── Load ────────────────────────────────────────────────────────────────
    async loadAll() {
      this.loading = true
      this.error = null
      try {
        // 1. Check if we already have saved data in Local Storage
        const storedJobs = localStorage.getItem('sjvms_jobs')
        const storedUsers = localStorage.getItem('sjvms_users')
        const storedApps = localStorage.getItem('sjvms_applications')

        if (storedJobs && storedUsers && storedApps) {
          // Use saved data if it exists
          this.jobs = JSON.parse(storedJobs)
          this.users = JSON.parse(storedUsers)
          this.applications = JSON.parse(storedApps)
        } else {
          // 2. Otherwise, fetch the default JSON files for the first time
          const [j, u, a] = await Promise.all([
            loadJSON<Job[]>('/data/jobs.json'),
            loadJSON<User[]>('/data/users.json'),
            loadJSON<Application[]>('/data/applications.json'),
          ])
          this.jobs = j
          this.users = u
          this.applications = a
          
          // Save these defaults to Local Storage so future edits apply to them
          this.saveData()
        }
      } catch (e: any) {
        this.error = e.message
      } finally {
        this.loading = false
      }
    },

    // ── Jobs ────────────────────────────────────────────────────────────────
    addJob(job: Omit<Job, 'id'>) {
      const id = this.jobs.length ? Math.max(...this.jobs.map(j => j.id)) + 1 : 1
      this.jobs.push({ ...job, id })
      this.saveData() // Save to Local Storage
    },
    updateJob(updated: Job) {
      const idx = this.jobs.findIndex(j => j.id === updated.id)
      if (idx !== -1) {
        this.jobs[idx] = updated
        this.saveData()
      }
    },
    removeJob(id: number) {
      this.jobs = this.jobs.filter(j => j.id !== id)
      this.saveData()
    },
    toggleJobStatus(id: number) {
      const job = this.jobs.find(j => j.id === id)
      if (job) {
        job.isOpen = !job.isOpen
        this.saveData()
      }
    },
    getJobById(id: number) {
      return this.jobs.find(j => j.id === id) ?? null
    },
    searchJobs(query: string, category: string, remote: boolean | null) {
      const q = query.toLowerCase()
      return this.jobs.filter(j => {
        const matchQuery =
          !q ||
          j.title.toLowerCase().includes(q) ||
          j.company.toLowerCase().includes(q) ||
          j.location.toLowerCase().includes(q) ||
          j.requiredSkills.some(s => s.toLowerCase().includes(q))
        const matchCat = !category || j.category === category
        const matchRemote = remote === null || j.isRemote === remote
        return matchQuery && matchCat && matchRemote
      })
    },
    getMatchingJobsForUser(userSkills: string[]) {
      return this.jobs
        .filter(j => j.isOpen)
        .map(j => {
          const matched = j.requiredSkills.filter(s =>
            userSkills.map(x => x.toLowerCase()).includes(s.toLowerCase()),
          )
          return { job: j, matchScore: matched.length / Math.max(j.requiredSkills.length, 1) }
        })
        .filter(x => x.matchScore > 0)
        .sort((a, b) => b.matchScore - a.matchScore)
    },

    // ── Users ───────────────────────────────────────────────────────────────
    registerUser(user: Omit<User, 'id'>) {
      if (this.users.some(u => u.username === user.username)) {
        throw new Error('Username already exists')
      }
      const id = this.users.length ? Math.max(...this.users.map(u => u.id)) + 1 : 1
      const newUser: User = { ...user, id }
      this.users.push(newUser)
      this.saveData() // Save to Local Storage
      return newUser
    },
    loginUser(username: string, password: string): User | null {
      return this.users.find(u => u.username === username && u.password === password) ?? null
    },
    updateUser(updated: User) {
      const idx = this.users.findIndex(u => u.id === updated.id)
      if (idx !== -1) {
        this.users[idx] = updated
        this.saveData()
      }
    },

    // ── Applications ────────────────────────────────────────────────────────
    submitApplication(app: Omit<Application, 'applicationId'>) {
      const applicationId = this.applications.length
        ? Math.max(...this.applications.map(a => a.applicationId)) + 1
        : 1
      this.applications.push({ ...app, applicationId })
      this.saveData() // Save to Local Storage
    },
    updateApplicationStatus(applicationId: number, status: Application['status']) {
      const app = this.applications.find(a => a.applicationId === applicationId)
      if (app) {
        app.status = status
        this.saveData()
      }
    },
    getApplicationsByUser(userId: number) {
      return this.applications.filter(a => a.userId === userId)
    },
    getApplicationsByJob(jobId: number) {
      return this.applications.filter(a => a.jobId === jobId)
    },
    hasApplied(userId: number, jobId: number) {
      return this.applications.some(a => a.userId === userId && a.jobId === jobId)
    }
  }
})